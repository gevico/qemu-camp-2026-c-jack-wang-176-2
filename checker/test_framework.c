/**
 * test_framework.c
 * * 强化版 C 语言测试框架实现 - 修复了长路径下缓冲区溢出的问题
 */

#include "test_framework.h"
#include <stdarg.h>

// 全局测试统计
test_stats_t g_test_stats = {0, 0, 0};

// 当前练习题结果
exercise_result_t g_current_exercise;

// 开始时间
static clock_t start_time;

/**
 * 初始化测试框架
 */
void test_init(const char* exercise_name) {
    g_test_stats.total_tests = 0;
    g_test_stats.passed_tests = 0;
    g_test_stats.failed_tests = 0;
    
    memset(&g_current_exercise, 0, sizeof(g_current_exercise));
    strncpy(g_current_exercise.exercise_name, exercise_name, sizeof(g_current_exercise.exercise_name) - 1);
    g_current_exercise.individual_test_count = 0;
    
    start_time = clock();
    
    printf("\n" COLOR_BLUE "🧪 开始运行测试..." COLOR_RESET "\n");
    printf("==================================================\n");
}

/**
 * 完成测试并显示结果
 */
void test_finish(void) {
    clock_t end_time = clock();
    g_current_exercise.test_duration_ms = (int)((end_time - start_time) * 1000 / CLOCKS_PER_SEC);
    g_current_exercise.stats = g_test_stats;
    
    if (g_test_stats.failed_tests == 0) {
        strcpy(g_current_exercise.status, "PASSED");
        g_current_exercise.compiled_successfully = 1;
    } else {
        strcpy(g_current_exercise.status, "FAILED");
    }
    
    printf("\n==================================================\n");
    printf(COLOR_BLUE "📊 测试结果统计:" COLOR_RESET "\n");
    printf("总测试数: %d\n", g_test_stats.total_tests);
    printf(COLOR_GREEN "通过: %d" COLOR_RESET "\n", g_test_stats.passed_tests);
    printf(COLOR_RED "失败: %d" COLOR_RESET "\n", g_test_stats.failed_tests);
    
    if (g_test_stats.failed_tests == 0) {
        printf("\n" COLOR_GREEN "🎉 所有测试都通过了！" COLOR_RESET "\n");
        exit(0);
    } else {
        printf("\n" COLOR_RED "❌ 有 %d 个测试失败" COLOR_RESET "\n", g_test_stats.failed_tests);
        exit(1);
    }
}

/**
 * 编译并运行程序，捕获输出
 */
int compile_and_run(const char* source_file, const char* executable, char* output, size_t output_size, int is_make) {
    // 🚨 关键修改：将缓冲区从 512/256 暴力扩容至 4096/2048，彻底解决路径截断问题
    char compile_cmd[4096]; 
    char run_cmd[2048];
    FILE* fp;
    
    memset(output, 0, output_size);
    
    switch (is_make) {
        case 1:
        case 2: {
            // 注意：这里使用的是相对于 tests 目录的路径
            snprintf(compile_cmd, sizeof(compile_cmd), "cd ../exercises/%s && make clean && make all 2>&1", executable);
            fp = popen(compile_cmd, "r");
            if (fp == NULL) {
                snprintf(output, output_size, "无法执行 popen 编译命令: %s", compile_cmd);
                return -1;
            }
        }
        break;
        default: {
            if (strstr(source_file, "prime") != NULL) {
                snprintf(compile_cmd, sizeof(compile_cmd), 
                        "cd ../exercises/%s && gcc -Wall -Wextra -std=c11 -o %s %s.c -lm 2>&1", executable, executable, executable);
            } else {
                snprintf(compile_cmd, sizeof(compile_cmd), 
                        "cd ../exercises/%s && gcc -Wall -Wextra -std=c11 -o %s %s.c 2>&1", executable, executable, executable);
            }
            fp = popen(compile_cmd, "r");
            if (fp == NULL) {
                snprintf(output, output_size, "无法执行 popen 编译命令: %s", compile_cmd);
                return -1;
            }
        }
    }
    
    // 读取编译输出（为了防止编译输出本身很大，这里缓冲区也给足）
    char compile_output[8192] = {0};
    fread(compile_output, 1, sizeof(compile_output) - 1, fp);
    int compile_status = pclose(fp);
    
    if (compile_status != 0) {
        // 🚨 调试神句：如果编译失败，把执行的完整命令也打出来
        snprintf(output, output_size, "编译失败(Exit:%d)。\n[执行命令]: %s\n[错误输出]:\n%s", 
                 compile_status, compile_cmd, compile_output);
        return -1;
    }
    
    // 运行程序阶段
    switch (is_make) {
        case 2: {
            // 🚨 修正运行路径：进入到练习题目录再跑你的自定义测试脚本
            snprintf(run_cmd, sizeof(run_cmd), "cd ../exercises/%s && bash ../../tests/run_my_test.sh 2>&1", executable);
            fp = popen(run_cmd, "r");
        }
        break;
        case 1:
        default: {
            // 确保 executable 名字正确（你在 Makefile 里改成了 20_mybash）
            snprintf(run_cmd, sizeof(run_cmd), "cd ../exercises/%s && ./%s 2>&1", executable, executable);
            fp = popen(run_cmd, "r");
        }
    }

    if (fp == NULL) {
        snprintf(output, output_size, "无法执行运行命令: %s", run_cmd);
        return -1;
    }

    // 读取程序实际输出
    size_t bytes_read = fread(output, 1, output_size - 1, fp);
    output[bytes_read] = '\0';
    
    int run_status = pclose(fp);
    return run_status;
}

/**
 * 检查字符串是否包含子字符串
 */
int string_contains(const char* haystack, const char* needle) {
    if (haystack == NULL || needle == NULL) return 0;
    return strstr(haystack, needle) != NULL;
}

/**
 * 检查文件是否包含特定标记
 */
int file_contains_marker(const char* filename, const char* marker) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) return 0;
    
    char line[1024]; // 扩容行缓冲区
    int found = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, marker) != NULL) {
            found = 1;
            break;
        }
    }
    fclose(file);
    return found;
}

/**
 * 添加单个测试结果
 */
void add_test_result(const char* name, const char* status, const char* message, int expected, int actual, int has_expected_actual) {
    if (g_current_exercise.individual_test_count >= 32) return;
    
    test_result_t* result = &g_current_exercise.individual_tests[g_current_exercise.individual_test_count];
    strncpy(result->name, name, sizeof(result->name) - 1);
    strncpy(result->status, status, sizeof(result->status) - 1);
    strncpy(result->message, message, sizeof(result->message) - 1);
    result->expected = expected;
    result->actual = actual;
    result->has_expected_actual = has_expected_actual;
    
    g_current_exercise.individual_test_count++;
}

/**
 * 生成JSON测试报告
 */
void generate_json_report(const char* filename, exercise_result_t* result) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) return;
    
    time_t now = time(NULL);
    struct tm* tm_info = gmtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", tm_info);
    
    fprintf(file, "{\n  \"test_session\": {\n    \"timestamp\": \"%s\",\n", timestamp);
    fprintf(file, "    \"exercise_name\": \"%s\"\n  },\n", result->exercise_name);
    fprintf(file, "  \"exercise\": {\n    \"status\": \"%s\",\n", result->status);
    fprintf(file, "    \"passed_tests\": %d,\n    \"failed_tests\": %d\n  }\n}\n", 
            result->stats.passed_tests, result->stats.failed_tests);
    
    fclose(file);
}