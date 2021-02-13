/*
Compile with:

$CXX_COMPILER \
-g \
--target=wasm32 \
--no-standard-libraries \
-Xlinker --import-memory \
-Xlinker --no-entry \
-Xlinker --export=Sum \
-Xlinker --export=Hello \
-fvisibility=hidden \
-Xlinker --allow-undefined-file=./js_fct.syms \
-o MyTest.wasm test.cpp
*/

extern "C" {
  // Wrapper for our JavaScript function
  extern void console_log_int(int sum);
  extern void console_log_str(const char*);

  int Sum(int a, int b) {
    int sum = a + b;
    console_log_str("Sum: ");
    console_log_int(sum);
    return sum;
  }

  void Hello() {
    console_log_str("Hello from WASM.");
  }
}

// __declspec(dllexport) void log(int sum) {
//   std::cout << sum << std::endl;
// }

// int main() {
//   // int sum = Sum(1,3);
//   // log(12);
//   // std::cout << "Hello JS!" << std::endl;
//   return 0;
// }