#include <thread>
#include <vector>
#include <filesystem>
#include <chrono>
#include <queue>
#include <mutex>
#include <iostream>
#include <condition_variable>
#include <cstring>
#include <functional>
#include <args_parse/args.hpp>

/// @brief Структура для представления дерева каталогов
struct Directory {
	// Имя каталога
	std::string name;
	// Имена файлов
	std::vector<std::string> files;
	// Подкаталоги
	std::vector<Directory> subdirectories;
	// Номер потока, который обрабатывал директорию
	std::thread::id threadId;

	// Конструктор
	Directory(const std::string& name) : name(name) {}
};

/// @brief Функция для вывода дерева каталогов
void printDirectory(const Directory& directory) {
	// Вывод подкаталогов
	for (const auto& subdir : directory.subdirectories) {
		std::cout << "\t" << subdir.name << " (Thread ID: " << subdir.threadId << ")" << std::endl;
		printDirectory(subdir);
	}
	// Вывод файлов
	for (const auto& file : directory.files) {
		std::cout << "\t\t" << file << std::endl;
	}
}
/// @brief Класс управления пулом потоков
class ThreadPool {
public:
	// Конструтор
	ThreadPool(size_t numThreads) : stopped(false) {
		for (size_t i = 0; i < numThreads; ++i) {
			workers.emplace_back([this] {
				while (true) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(queueMutex);
						// Ждем, пока очередь задач не станет непустой или не установлен флаг stopped
						condition.wait(lock, [this] { return stopped || !tasks.empty(); });
						// Поток завершает работу, если флаг stop установлен и очередь задач пуста
						if (stopped && tasks.empty()) return;
						task = std::move(tasks.front());
						tasks.pop();
					}
					task();
				}
				});
		}
	}

	/// @brief Метод для добавления задачи в очередь
	template<class F>
	void enqueue(F&& f) {
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			tasks.emplace(std::forward<F>(f));
		}
		condition.notify_one();
	}

	/// @brief Деструктор
	~ThreadPool() {
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			stopped = true;
		}
		condition.notify_all(); // Разбудить все потоки, чтобы они могли завершиться
		for (std::thread& worker : workers) {
			worker.join();
		}
	}
	/// @brief Метод для проверки, остановлен ли ThreadPool
	bool isStopped() const {
		return stopped.load();
		//return stop;
	}

private:
	// Вектор потоков
	std::vector<std::thread> workers;
	// Очередь задач
	std::queue<std::function<void()>> tasks;
	// Мьютекс для синхронизации доступа к очереди
	std::mutex queueMutex;
	// Условная переменная для управления потоками
	std::condition_variable condition;
	// Атомарная переменная для определения состояния остановки
	std::atomic<bool> stopped;
};
/// @brief Выполнения задачи обработки каталога
class Task {
public:
	// Конструктор
	Task(const std::string& path, ThreadPool& pool) : path(path), pool(pool) {}
	// Оператор вызова для выполнения задачи
	void operator()() {
		processDirectory(path);
	}

private:
	// Путь к каталогу
	std::string path;
	// Пул потоков
	ThreadPool& pool;
	/// @brief Обработка каталога
	void processDirectory(const std::string& path) {
		std::string directoryName = std::filesystem::path(path).filename().string();
		Directory directory(directoryName);
		directory.threadId = std::this_thread::get_id();
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			if (entry.is_directory() && entry.path().filename().string()[0] != '.') {
				// Получаем имя подкаталога
				std::string subdirName = entry.path().filename().string();
				// Добавляем задачу для обработки подкаталога в пул
				pool.enqueue(Task(entry.path().string(), pool));
				directory.subdirectories.emplace_back(subdirName);
			}
			else if (entry.is_regular_file()) {
				// Добавляем имя файла в список файлов каталога
				directory.files.push_back(entry.path().filename().string());
			}
		}
		// Вывод структуры каталога
		printDirectory(directory);
	}
};

int main(int argc, const char** argv) {
	// Парсинг аргументов командной строки
	args_parse::ArgsParser parser;
	args_parse::SingleArg<std::string> path('p', "path");
	args_parse::SingleArg<int> threads('t', "threads");

	path.SetDescription("single string argument to set root path");
	threads.SetDescription("single string argument to set amount of threads");

	parser.add(&path);
	parser.add(&threads);

	parser.parse(argc, argv);
	parser.printHelp();

	// Проверка наличия аргументов
	if (path.isDefined() && threads.isDefined()) {
		std::cerr << "Usage: " << "  -p <directory_path> -t <num_threads>\n";
	}
	// Проверка, является ли указанный путь директорией
	if (!std::filesystem::is_directory(path.value())) {
		std::cerr << "Error: Not a valid directory\n";
		return 1;
	}
	// Создание пула потоков и задачи для обработки корневого каталога
	ThreadPool pool(threads.value());
	Task task(path.value(), pool);
	// Добавляем задачу в пул
	pool.enqueue(std::ref(task));
	//pool.~ThreadPool();
	while (!pool.isStopped()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	return 0;
}