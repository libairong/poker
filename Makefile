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

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

