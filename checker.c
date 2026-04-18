/**
 * checker.c - 高兼容性大容量版
 * 修复了在深层路径环境下命令被截断导致的编译失败问题
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_EXERCISES 40
#define MAX_NAME_LEN 128      // 增加名字长度上限
#define SCORE_PER_EXERCISE 5
#define TOTAL_SCORE 200
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_BLUE "\033[0;34m"
#define COLOR_RESET "\033[0m"

typedef struct {
    char name[MAX_NAME_LEN];
    int completed;
} exercise_t;

typedef struct {
    exercise_t exercises[MAX_EXERCISES];
    int count;
    int total_passed;
    int total_failed;
} checker_t;

static void generate_overall_json_report(checker_t *checker);

static int file_contains_marker(const char *filename, const char *marker) {
    FILE *file = fopen(filename, "r");
    char line[1024]; // 增加行读取缓冲区

    if (file == NULL) {
        return 0;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strstr(line, marker) != NULL) {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

static void get_primary_source_path(const char *exercise_name, char *filepath, size_t filepath_size) {
    if (strcmp(exercise_name, "20_mybash") == 0) {
        snprintf(filepath, filepath_size, "exercises/20_mybash/src/mybash/main.c");
        return;
    }

    snprintf(filepath, filepath_size, "exercises/%s/%s.c", exercise_name, exercise_name);
}

static int run_test(const char *exercise_name) {
    char test_path[1024];    // 扩容：支持深层路径
    char compile_cmd[4096];  // 核心扩容：防止 GCC 命令被截断
    char run_cmd[2048];      // 扩容
    char cleanup_cmd[1024];  // 扩容
    int compile_result;
    int test_result;

    snprintf(test_path, sizeof(test_path), "tests/test_%s.c", exercise_name);
    if (access(test_path, F_OK) != 0) {
        printf(COLOR_YELLOW "⚠️  没有找到测试文件: %s" COLOR_RESET "\n", test_path);
        return -1;
    }

    // 🚨 移除了 2>/dev/null，确保错误信息能正常输出到 GitHub Actions 日志
    snprintf(
        compile_cmd,
        sizeof(compile_cmd),
        "cd tests && gcc -Wall -Wextra -g -std=c11 -o test_%s test_%s.c ../checker/test_framework.c -I../checker",
        exercise_name,
        exercise_name);
    
    compile_result = system(compile_cmd);
    if (compile_result != 0) {
        printf(COLOR_RED "❌ 测试编译失败: %s (错误码: %d)" COLOR_RESET "\n", exercise_name, compile_result);
        return -1;
    }

    // 运行测试
    snprintf(run_cmd, sizeof(run_cmd), "cd tests && ./test_%s", exercise_name);
    test_result = system(run_cmd);

    // 只有测试通过才清理，失败则保留现场供调试
    if (test_result == 0) {
        snprintf(cleanup_cmd, sizeof(cleanup_cmd), "cd tests && rm -f test_%s", exercise_name);
        system(cleanup_cmd);
    } else {
        printf(COLOR_YELLOW "⚠️  测试失败或发生段错误！已保留二进制文件 tests/test_%s" COLOR_RESET "\n", exercise_name);
    }

    return test_result == 0 ? 0 : 1;
}

static int check_exercise(const char *exercise_name) {
    char filepath[1024]; // 扩容
    int test_result;

    get_primary_source_path(exercise_name, filepath, sizeof(filepath));

    printf("\n" COLOR_BLUE "🔍 检查练习题: %s" COLOR_RESET "\n", exercise_name);
    printf("================================================\n");

    if (access(filepath, F_OK) != 0) {
        printf(COLOR_RED "❌ 文件不存在: %s" COLOR_RESET "\n", filepath);
        return 0;
    }

    if (file_contains_marker(filepath, "I AM NOT DONE")) {
        printf(COLOR_YELLOW "⏳ 练习题尚未完成，请移除 'I AM NOT DONE' 标记" COLOR_RESET "\n");
        return 0;
    }

    test_result = run_test(exercise_name);
    if (test_result == 0) {
        printf(COLOR_GREEN "✅ 练习题通过所有测试！" COLOR_RESET "\n");
        return 1;
    }
    if (test_result == 1) {
        printf(COLOR_RED "❌ 练习题测试失败" COLOR_RESET "\n");
        return -1;
    }

    printf(COLOR_YELLOW "⚠️  无法运行测试" COLOR_RESET "\n");
    return 0;
}

static void list_exercises(checker_t *checker) {
    int i;
    printf(COLOR_BLUE "📚 可用的练习题:" COLOR_RESET "\n");
    printf("================================================\n");
    for (i = 0; i < checker->count; i++) {
        char filepath[1024];
        get_primary_source_path(checker->exercises[i].name, filepath, sizeof(filepath));
        if (file_contains_marker(filepath, "I AM NOT DONE")) {
            printf("  %s - " COLOR_RED "❌ 未完成" COLOR_RESET "\n", checker->exercises[i].name);
        } else {
            printf("  %s - " COLOR_GREEN "✅ 已完成" COLOR_RESET "\n", checker->exercises[i].name);
        }
    }
}

static void check_all_exercises(checker_t *checker) {
    int i;
    printf(COLOR_BLUE "🧪 C 语言练习题检查器 (高兼容路径版)" COLOR_RESET "\n");
    printf("================================================\n");

    checker->total_passed = 0;
    checker->total_failed = 0;

    for (i = 0; i < checker->count; i++) {
        int result = check_exercise(checker->exercises[i].name);
        if (result == 1) {
            checker->total_passed++;
            checker->exercises[i].completed = 1;
        } else {
            checker->total_failed++;
            checker->exercises[i].completed = 0;
        }
    }

    printf("\n" COLOR_BLUE "📊 总结:" COLOR_RESET "\n");
    printf("总共 %d 道练习题\n", checker->count);
    printf(COLOR_GREEN "通过 %d 道" COLOR_RESET "\n", checker->total_passed);
    printf(COLOR_RED "失败 %d 道" COLOR_RESET "\n", checker->total_failed);
    printf(COLOR_BLUE "🏆 总分数: %d/%d" COLOR_RESET "\n", checker->total_passed * SCORE_PER_EXERCISE, TOTAL_SCORE);

    generate_overall_json_report(checker);

    if (checker->total_passed == checker->count) {
        printf("\n" COLOR_GREEN "🎉 恭喜！所有练习题都通过了！" COLOR_RESET "\n");
    }
}

// generate_overall_json_report, find_exercise_by_number, init_exercises, show_help, show_hint 
// 这些函数保持逻辑不变，但内部使用扩容后的常量
static void generate_overall_json_report(checker_t *checker) {
    FILE *file = fopen("test_results_summary.json", "w");
    if (file == NULL) return;

    time_t now = time(NULL);
    struct tm *tm_info = gmtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", tm_info);

    fprintf(file, "{\n  \"test_summary\": {\n");
    fprintf(file, "    \"total_exercises\": %d,\n", checker->count);
    fprintf(file, "    \"passed_exercises\": %d,\n", checker->total_passed);
    fprintf(file, "    \"total_score\": %d\n  },\n", checker->total_passed * SCORE_PER_EXERCISE);
    fprintf(file, "  \"exercises\": [\n");

    for (int i = 0; i < checker->count; i++) {
        fprintf(file, "    {\"name\": \"%s\", \"status\": \"%s\"}%s\n", 
                checker->exercises[i].name, 
                checker->exercises[i].completed ? "PASSED" : "FAILED",
                (i < checker->count - 1) ? "," : "");
    }
    fprintf(file, "  ]\n}\n");
    fclose(file);
}

// 保持其余辅助函数不变...
static const char *find_exercise_by_number(checker_t *checker, const char *number) {
    for (int i = 0; i < checker->count; i++) {
        if (strstr(checker->exercises[i].name, number)) return checker->exercises[i].name;
    }
    return NULL;
}

static void init_exercises(checker_t *checker) {
    const char *names[] = {
        "01_insert_sort", "02_merge_sort", "03_quick_sort", "04_linear_search", "05_binary_search",
        "06_stack_maze", "07_queue_maze", "08_circular_queue", "09_word_counter", "10_my_strcpy",
        "11_command_interpreter", "12_student_management", "13_universal_sorter", "14_calculator", "15_url_parser",
        "16_mysed", "17_myfile", "18_mywc", "19_mytrans", "20_mybash",
        "21_singly_linked_list_josephus", "22_doubly_circular_queue", "23_circular_linked_list_josephus", "24_prev_binary_tree", "25_counter_letter",
        "26_hash_counter", "27_asm_gcd", "28_operator_overflow", "29_swap_endian", "30_debug_print",
        "31_event_handler", "32_container_of_macro", "33_garray_dynamic_array", "34_protocol_header_parser", "35_elf_info_parser",
        "36_lru_cache", "37_bitmap_operations", "38_thread_safe_ring_buffer", "39_strtok_r_thread_safe", "40_bloom_filter_bitmap"
    };
    checker->count = 40;
    for (int i = 0; i < 40; i++) {
        strncpy(checker->exercises[i].name, names[i], MAX_NAME_LEN);
        checker->exercises[i].completed = 0;
    }
}

int main(int argc, char *argv[]) {
    checker_t checker;
    init_exercises(&checker);
    if (argc < 2) return 1;
    if (strcmp(argv[1], "check-all") == 0) check_all_exercises(&checker);
    else if (strcmp(argv[1], "check") == 0 && argc == 3) {
        const char *ex = find_exercise_by_number(&checker, argv[2]);
        if (ex) return check_exercise(ex) == 1 ? 0 : 1;
    }
    return 0;
}