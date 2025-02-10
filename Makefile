# 定义编译器和编译选项
CXX = g++
CXXFLAGS = -std=c++20 -Wall -g -O2

# 定义源文件和目标文件
SRC_DIRS = src src/7g523 src/display
SRC = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))
OBJ_DIR = obj
OBJ = $(SRC:$(SRC_DIRS)/%.cpp=$(OBJ_DIR)/%.o)
EXEC = my_game

# 默认目标
all: $(EXEC)

# 链接目标
$(EXEC): $(OBJ)
	@echo "Linking object files to create the final executable: $(EXEC)"
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(EXEC)
	@echo "Build completed."

# 编译源文件并生成依赖文件
$(OBJ_DIR)/%.o: $(SRC_DIRS)/%.cpp | $(OBJ_DIR)
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Finished compiling: $<"

# 确保 obj 目录存在
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# 清理目标文件和可执行文件
clean:
	@echo "Cleaning up the build directory..."
	rm -f $(OBJ_DIR)/*.o $(EXEC)
	@echo "Clean completed."

# 安装目标
install: $(EXEC)
	@echo "Installing program to /usr/local/bin"
	cp $(EXEC) /usr/local/bin/
	@echo "Installation completed."

# 打印当前的变量值（可选）
print:
	@echo "SRC_DIRS = $(SRC_DIRS)"
	@echo "SRC = $(SRC)"
	@echo "OBJ = $(OBJ)"
	@echo "EXEC = $(EXEC)"

# 编译调试版本
debug: CXXFLAGS += -g
debug: $(EXEC)

# 编译发布版本
release: CXXFLAGS += -O2
release: $(EXEC)

