CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRCDIR = Src
INCDIR = Include
BINDIR = .

# Source files
SOURCES = $(SRCDIR)/Lagrange.cpp \
          $(SRCDIR)/WeatherDataHandler.cpp \
          $(SRCDIR)/GnuplotHelper.cpp \
          main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = lagrange_interpolation

# Default target
all: $(EXECUTABLE)

# Build executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -o $@ $^
	@echo "✓ Compilation successful!"
	@echo "Run: ./$(EXECUTABLE)"

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Clean build
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
	@echo "✓ Clean complete"

# Help target
help:
	@echo "Available targets:"
	@echo "  make        - Compile the project"
	@echo "  make clean  - Remove compiled files"
	@echo "  make run    - Compile and run"
	@echo "  make help   - Show this help message"

# Run target
run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean help run
