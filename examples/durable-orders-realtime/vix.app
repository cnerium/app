name = "durable-orders-realtime"
type = "executable"
standard = "c++20"

sources = [
  "src/main.cpp"
]

include_dirs = [
  "../../include"
]

links = [
  "cnerium"
]

output_dir = "build"
