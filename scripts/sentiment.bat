@echo off

cd C:\ancient_futures\LLM-emotion-color-parser

call venv\Scripts\activate

call set CO_API_KEY=%1

python src/claude_instructor_test/__init__.py %2

call venv\Scripts\deactivate

:done
echo.
