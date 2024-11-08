#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdbool.h>
#include "types.h"

// 설치 관련 명령어
CommandResult delete_logs_and_restart();
CommandResult install_without_user_input();
CommandResult install_with_user_input();
CommandResult connect_to_server(const char *server_url);

// 화이트리스트 관련 명령어
CommandResult add_to_whitelist(const char *path);
CommandResult add_hash_to_whitelist(const char *path);
CommandResult delete_from_whitelist(const char *path);
CommandResult delete_hash_from_whitelist(const char *hash);

// 로깅 관련 명령어
CommandResult set_log_level(int level);
CommandResult simulate_log(const char *filename);
CommandResult simulate_operation(const char *filename);
CommandResult delete_operation_logs();

// 보안 관련 명령어
CommandResult set_exit_password();
CommandResult exit_with_password(const char *password);
CommandResult set_uninstall_password();
CommandResult uninstall_with_password(const char *password);

// 테스트 관련 명령어
CommandResult test_environment(const char *config);
CommandResult register_selinux();
CommandResult run_test(int test_number, int repeat_count);

// 도움말 표시
void display_help(bool first_section);

#endif