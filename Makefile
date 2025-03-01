# 定义编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++20 -Wall -g -O2

# 定义源文件目录和目标文件目录
SRC_DIRS = src src/7g523 src/display
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
DEP_DIR = $(BUILD_DIR)/dep

# 获取所有源文件
SRC = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

# 生成目标文件和依赖文件路径
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEP = $(SRC:%.cpp=$(DEP_DIR)/%.d)

# 最终可执行文件
EXEC = $(BUILD_DIR)/my_game

# 默认目标
all: $(EXEC)

# 链接目标文件生成可执行文件
$(EXEC): $(OBJ)
	@echo "Linking object files to create the final executable: $(EXEC)"
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)
	@echo "Build completed."

# 编译源文件并生成目标文件和依赖文件
$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)  # 动态创建目标目录
	@mkdir -p $(DEP_DIR)/$(dir $<)  # 动态创建依赖文件目录
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@
	@echo "Finished compiling: $<"

# 确保目标文件和依赖文件目录存在
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(DEP_DIR):
	mkdir -p $(DEP_DIR)

# 清理生成的文件
clean:
	@echo "Cleaning up the build directory..."
	rm -rf $(BUILD_DIR)
	@echo "Clean completed."

# 安装可执行文件到 /usr/local/bin
install: $(EXEC)
	@echo "Installing program to /usr/local/bin"
	cp $(EXEC) /usr/local/bin/
	@echo "Installation completed."

# 打印变量值（可选）
print:
	@echo "SRC_DIRS = $(SRC_DIRS)"
	@echo "SRC = $(SRC)"
	@echo "OBJ = $(OBJ)"
	@echo "DEP = $(DEP)"
	@echo "EXEC = $(EXEC)"

# 编译调试版本
debug: CXXFLAGS += -g
debug: $(EXEC)

# 编译发布版本
release: CXXFLAGS += -O2
release: $(EXEC)

# 包含自动生成的依赖文件
-include $(DEP)

