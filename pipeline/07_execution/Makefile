# Makefile for Stage 7: Execution
# This stage represents the final execution of compiled programs

# No compilation needed - execution is handled by the system
all:
	@echo "Execution stage - no compilation needed"
	@echo "This stage represents running the final executables"

# Test execution by running the complete pipeline
test:
	@echo "Testing complete pipeline execution..."
	@echo "Building all previous stages..."
	@cd ../01_lexical_analysis && make all
	@cd ../02_syntax_analysis && make all
	@cd ../04_code_generation && make all
	@cd ../05_assembly && make all
	@cd ../06_linking && make all
	@echo ""
	@echo "Running complete pipeline tests..."
	@cd ../01_lexical_analysis && make test
	@cd ../02_syntax_analysis && make test
	@cd ../04_code_generation && make test
	@cd ../05_assembly && make test
	@cd ../06_linking && make test
	@echo ""
	@echo "Pipeline execution test complete!"

# Clean - clean all stages
clean:
	@echo "Cleaning all pipeline stages..."
	@cd ../01_lexical_analysis && make clean
	@cd ../02_syntax_analysis && make clean
	@cd ../04_code_generation && make clean
	@cd ../05_assembly && make clean
	@cd ../06_linking && make clean

# Install - install all stages
install:
	@echo "Installing all pipeline stages..."
	@cd ../01_lexical_analysis && make install
	@cd ../02_syntax_analysis && make install
	@cd ../04_code_generation && make install
	@cd ../05_assembly && make install
	@cd ../06_linking && make install

.PHONY: all test clean install
