./args_parse_demo -h -g10 -ooutput_file.txt --float 3.14 -u 500000n -a 1 2 3 -s hello world -b true false true -m 1.5 2.5 3.5

  -u, --user    single user argument shows time converted to microseconds
  
  -h, --help    single bool argument shows the list of the arguments
  
  -g, --getNumber    single int argument shows Input g value [int]
  
  -a, --age    multiple int argument shows Input g value [int] [int] [int]...
  
  -o, --output    single string argument shows Output o value [string]
  
  -b, --bool    multiple bool argument shows Input multi bool values: [bool] [bool] [bool]...
  
  -f, --float    single float argument shows Input f value [float]
  
  -s, --str    multiple string argument shows Output o value [string] [string] [string]...
  
  -m, --mflo    multiple float argument shows Input f value [float] [float] [float]...
