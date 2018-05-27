#include <getopt.h>
#include <iostream>
#include "area776.hpp"

const std::string Version = "v0.0.1";

bool parse_options(const int argc, char **argv) noexcept {
  bool ret = false;
  opterr = 0;
  const option long_options[] = {
      {"debug", no_argument, nullptr, 'd'},
      {"help", no_argument, nullptr, 'h'},
      {"version", no_argument, nullptr, 'v'},
      {nullptr, 0, nullptr, 0},
  };

  for (;;) {
    const int curind = optind;
    const int c = getopt_long(argc, argv, "dhv", long_options, nullptr);
    if (c == -1) {
      break;
    }

    switch (c) {
      case 'h':
        std::cout << R"(Usage: area776 [options]

Options:
    -d  --debug         debug mode
    -h, --help          print this help menu
    -v, --version       print version
)";
        exit(EXIT_SUCCESS);
      case 'd':
        ret = true;
        break;
      case 'v':
        std::cout << Version << '\n';
        exit(EXIT_SUCCESS);
      case '?': {
        std::string av(argv[curind]);
        int n = 0;
        while (av[n] == '-') {
          ++n;
        }
        av.erase(av.begin(), av.begin() + n);
        std::cerr << "Unrecognized option: '" << av << "'\n";
        exit(EXIT_FAILURE);
      }
      default:
        // do nothing
        break;
    }
  }

  return ret;
}

int main(int argc, char **argv) {
  const bool debug_mode = parse_options(argc, argv);
  Area776 game(debug_mode);
  if (!game.init()) {
    exit(EXIT_FAILURE);
  }
  game.main_loop();
  exit(EXIT_SUCCESS);
}
