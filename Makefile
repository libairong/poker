# Makefile for card game

CC := gcc
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Wpedantic

SRCDIR := src
OBJDIR := obj
BINDIR := bin

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
TARGET := $(BINDIR)/card_game

# 创建目标目录
MKDIR_P = mkdir -p

# Debug模式CFLAGS
CFLAGS_DEBUG = -g
CXXFLAGS_DEBUG = $(CXXFLAGS) $(CFLAGS_DEBUG)

# Release模式CFLAGS
CFLAGS_RELEASE =
CXXFLAGS_RELEASE = $(CXXFLAGS) $(CFLAGS_RELEASE)

.PHONY: all clean debug release

all: release

release: CXXFLAGS := $(CXXFLAGS_RELEASE)
release: $(TARGET)

debug: CXXFLAGS := $(CXXFLAGS_DEBUG)
debug: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 创建目录
$(BINDIR):
	$(MKDIR_P) $(BINDIR)

$(OBJDIR):
	$(MKDIR_P) $(OBJDIR)

clean:
	rm -f $(OBJECTS) $(TARGET)

