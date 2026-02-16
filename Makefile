# Makefile for pg_function_catalog extension
# Extension name (must match .control file)
EXTENSION = pg_function_catalog
# Version-specific SQL files
DATA = sql/pg_function_catalog--1.0.sql
# C modules to compile (without .c extension)
MODULES = src/pg_function_catalog
# Documentation files to install
# DOCS = README.md
# Use PostgreSQL Extension Building Infrastructure (PGXS)
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
# Optional: Add compiler flags
# PG_CPPFLAGS = -Wall -Wextra