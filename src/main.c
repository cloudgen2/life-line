#include <libgen.h>       /* for basename */
#include <stdio.h>        /* for fprintf, stderr */ 
#include "fix-docroot.h"
#include "handle-exit.h"
#include "life-line.h"
#include "log-message.h"
#include "make-directory.h"
#include "project.h"
#include "remove-old-log.h"
#include "sync-key.h"

/**
 * @brief LifeLine - Prevents Docker container exit and manages SSH keys for the root account.
 * 
 * LifeLine is a program designed to ensure the continuous operation of Docker containers
 * by preventing them from exiting and maintaining a useful set of SSH keys for the root account.
 * 
 * The main function creates a directory to be monitored and enters an infinite loop to check for changes
 * to the directory. Each iteration of the loop is logged using the `log_message` function, and the program
 * gracefully exits when a SIGINT or SIGTERM signal is received.
 * 
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * 
 * @return int Returns 0 if the program runs successfully.
 * 
 * @details The function initializes the log, creates the root SSH folder, sets up the signal handler for graceful exit,
 * and starts the life line loop. It also checks for command-line arguments to enable debug mode or perform specific actions
 * such as printing the version or generating a simple log.
 * 
 * @note This function requires the following include files:
 * @note #include <stdio.h> // for fprintf, stderr
 * 
 * @see handle_exit - Signal handler function
 * @see make_directory - Function to create a directory
 * @see remove_old_logs - Function to remove old logs from a directory
 * 
 * @author Cloudgen Wong
 * @date 2023-06-06
 */
int main(int argc, char *argv[]) {
  char *thread_name = "Thread_main";
  int debug_mode = 0;
  char *me = basename(argv[0]);

  if(strcmp(me, "ll-log-msg") == 0) {
    if(argc == 3) {
      simple_log(argv[1], argv[2]);
    } else if (argc == 4) {
      int pid = atoi(argv[1]);
      simple_log_with_pid(pid, argv[2], argv[3]);
    } else if(argc == 2 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0 ||  strcmp(argv[1], "version") == 0)) {
      printf("%s(%s) version:%s\n", me, APP_NAME, APP_VERSION );
    }
  } else if(strcmp(me, "ll-log-file") == 0) {
    if(argc == 2) {
      if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0 ||  strcmp(argv[1], "version") == 0) {
        printf("%s(%s) version:%s\n", me, APP_NAME, APP_VERSION );
      } else {
        char *filename=logFilenameWithTimeStamp(debug_mode, prepareLogDir(DATA_LOG, argv[1]), argv[1]);
        printf("%s\n", filename );
        free(filename);
      }
    }
  } else if(strcmp(me, "ll-pid-file") == 0) {
    if(argc == 2) {
      if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0 ||  strcmp(argv[1], "version") == 0) {
        printf("%s(%s) version:%s\n", me, APP_NAME, APP_VERSION );
      } else {
        printf("/var/run/%s\n", argv[1] );
      }
    }
  } else if(strcmp(me, "ll-remove-old-log") == 0) {
    if(argc == 2) {
      if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0 ||  strcmp(argv[1], "version") == 0) {
        printf("%s(%s) version:%s\n", me, APP_NAME, APP_VERSION );
        return 0;
      } else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--debug") == 0 ||  strcmp(argv[1], "swvyf") == 0) {
        debug_mode = 1;
      }
    }
    remove_old_logs_with_debug(DATA_LOG, ".log", thread_name, debug_mode);
  } else if(strcmp(me, "ll-sync-key") == 0) {
    if(argc == 2) {
      if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0 ||  strcmp(argv[1], "version") == 0) {
        printf("%s(%s) version:%s\n", me, APP_NAME, APP_VERSION );
        return 0;
      } else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--debug") == 0 ||  strcmp(argv[1], "swvyf") == 0) {
        debug_mode = 1;
      }
    } 
    syncKey(DATA_PRIVATE_KEY, DATA_PUBLIC_KEY, ROOT_PRIVATE_KEY, ROOT_PUBLIC_KEY, thread_name, debug_mode);
    
  } else if(strcmp(me, "ll-fix-docroot") == 0) {
    if(argc == 2) {
      if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0 ||  strcmp(argv[1], "version") == 0) {
        printf("%s(%s) version:%s\n", me, APP_NAME, APP_VERSION );
      } else if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--debug") == 0 ||  strcmp(argv[1], "swvyf") == 0) {
        debug_mode = 1;
      }
    } 
    fixDocRoot(thread_name, debug_mode);
  } else {
    if (argc == 2) {
      if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0 ||  strcmp(argv[1], "version") == 0) {
        printf("%s(%s) version:%s\n", me, APP_NAME, APP_VERSION );
        return 0;
      } else if(strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-F") == 0 || strcmp(argv[1], "--debug") == 0 || strcmp(argv[1], "debug") == 0) {
        debug_mode = 1;
      } else if(strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--shortlinnk") == 0 || strcmp(argv[1], "shortlink") == 0) {
        lifeLifeShortLink(thread_name, 1);
        printf("Shortlinks have been ..Updated..\n");
        return 0;
      }
    }else if (argc == 3){
      if (strcmp(argv[1], "-o") == 0 || strcmp(argv[1], "--logfile") == 0 ||  strcmp(argv[1], "logfile") == 0) {
        char *filename=logFilenameWithTimeStamp(debug_mode, prepareLogDir(DATA_LOG, argv[2]), argv[2]);
        printf("%s\n", filename );
        free(filename);
        return 0;
      }
    } else if (argc == 4 && (strcmp(argv[1], "log") == 0 || strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--log") == 0)) {
      simple_log(argv[2], argv[3]);
      return 0;
    } else if (argc == 5 && (strcmp(argv[1], "log") == 0 || strcmp(argv[1], "-l") == 0 || strcmp(argv[1], "--log") == 0)) {
      int pid = atoi(argv[2]);
      simple_log_with_pid(pid, argv[3], argv[4]);
      return 0;  
    }
    init_log(thread_name);
    lifeLifeShortLink(thread_name, debug_mode);
    if (make_directory(ROOT_SSH) != 0) {
      log_message_w_thread( thread_name, "Creating folder: " ROOT_SSH " ..Failed..");
    }
    signal_exit();
    life_line(thread_name, debug_mode);
    if (argc == 1) {
      life_line_loop(thread_name, debug_mode);
    } else if (argc == 2 && (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "-F") == 0)) {
      life_line_loop(thread_name, debug_mode);
    }
  }
  return 0;
}
