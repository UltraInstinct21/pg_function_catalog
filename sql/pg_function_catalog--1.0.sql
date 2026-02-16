-- sql/pg_function_catalog--1.0.sql
-- Prevent script from being run directly in psql
\echo Use "CREATE EXTENSION pg_function_catalog" to load this file. \quit
-- Create the hello_catalog function
-- MODULE_PATHNAME is replaced with actual library path at install time
CREATE FUNCTION hello_catalog()
RETURNS text
AS 'MODULE_PATHNAME', 'hello_catalog'
LANGUAGE C STRICT IMMUTABLE;
-- Add a comment explaining what this function does
COMMENT ON FUNCTION hello_catalog() IS
 'Test function that returns a hello message';