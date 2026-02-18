-- test/test_catalog.sql
-- Comprehensive test suite for pg_function_catalog
-- Setup
\set ECHO all
\set ON_ERROR_STOP on
-- Clean start
DROP EXTENSION IF EXISTS pg_function_catalog CASCADE;
CREATE EXTENSION pg_function_catalog;
-- Test 1: Basic function lookup
\echo 'Test 1: Basic function lookup'
SELECT get_function_info('substring');
-- Test 2: Aggregate function
\echo 'Test 2: Aggregate function'
SELECT get_function_info('avg');
-- Test 3: Non-existent function
\echo 'Test 3: Non-existent function (should fail gracefully)'
SELECT get_function_info('nonexistent_func');
-- Test 4: Function with no arguments
\echo 'Test 4: Function with no arguments'
SELECT get_function_info('now');
-- Test 5: Window function
\echo 'Test 5: Window function'
SELECT get_function_info('row_number');
-- Test 6: Type conversion function
\echo 'Test 6: Type conversion'
SELECT get_function_info('cast');
-- Test 7: JSON functions
\echo 'Test 7: JSON function'
SELECT get_function_info('json_build_object');
-- Test 8: Array functions
\echo 'Test 8: Array function'
SELECT get_function_info('array_agg');
-- Test 9: Math functions
\echo 'Test 9: Math function'
SELECT get_function_info('sqrt');
-- Test 10: String functions
\echo 'Test 10: Multiple string functions'
SELECT get_function_info('lower');
SELECT get_function_info('upper');
SELECT get_function_info('trim');
\echo 'All tests completed!'