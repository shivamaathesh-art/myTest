PROGRAM mFBD_TestHarness
(*
 * Test Harness for mFBD Function Block
 * 
 * This program implements automated testing for the mFBD function block
 * Tests all 20 test cases defined in mFBD_TestSuite.md
 * 
 * Test Categories:
 *   TC001-TC004: Basic Functionality (4 tests)
 *   TC005-TC010: Boundary Value Analysis (6 tests)
 *   TC011-TC015: Edge Cases (5 tests)
 *   TC016-TC018: Output Validation (3 tests)
 *   TC019-TC020: Consistency (2 tests)
 * 
 * Author: QA/Test Team
 * Date: 2026-05-28
 *)

VAR
    (* Function Block Instance *)
    fb_test : myFBD;
    
    (* Test Management Variables *)
    test_count : INT := 0;
    pass_count : INT := 0;
    fail_count : INT := 0;
    current_test : STRING;
    
    (* Input Test Variables *)
    test_newVar1 : DINT;
    test_newVar2 : DINT;
    
    (* Expected Values *)
    expected_outVar : DINT;
    expected_x : BOOL;
    
    (* Actual Results *)
    actual_outVar : DINT;
    actual_x : BOOL;
    
    (* Result Variables *)
    test_passed : BOOL;
    output_match : BOOL;
    flag_match : BOOL;
    
END_VAR

(*
 ============================================
 TEST EXECUTION FUNCTION
 ============================================
 *)

PROCEDURE RunTest()
VAR
    temp_pass : BOOL;
END_VAR

    (* Set inputs *)
    fb_test.newVar1 := test_newVar1;
    fb_test.newVar2 := test_newVar2;
    
    (* Execute function block *)
    fb_test();
    
    (* Get actual results *)
    actual_outVar := fb_test.outVar;
    actual_x := fb_test.x;
    
    (* Compare with expected *)
    output_match := (actual_outVar = expected_outVar);
    flag_match := (actual_x = expected_x);
    test_passed := output_match AND flag_match;
    
    (* Update statistics *)
    test_count := test_count + 1;
    IF test_passed THEN
        pass_count := pass_count + 1;
    ELSE
        fail_count := fail_count + 1;
    END_IF;

END_PROCEDURE

(*
 ============================================
 TEST CASES
 ============================================
 *)

(*
 * TEST CATEGORY 1: Basic Functionality
 *)

PROCEDURE TC001()
(* Both Conditions TRUE *)
BEGIN
    current_test := 'TC001: Both Conditions TRUE';
    test_newVar1 := 15;  (* 15 >= 12 → TRUE *)
    test_newVar2 := 10;  (* 10 <= 12 → TRUE *)
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC002()
(* First TRUE, Second FALSE *)
BEGIN
    current_test := 'TC002: First TRUE, Second FALSE';
    test_newVar1 := 15;  (* 15 >= 12 → TRUE *)
    test_newVar2 := 15;  (* 15 <= 12 → FALSE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC003()
(* First FALSE, Second TRUE *)
BEGIN
    current_test := 'TC003: First FALSE, Second TRUE';
    test_newVar1 := 10;  (* 10 >= 12 → FALSE *)
    test_newVar2 := 10;  (* 10 <= 12 → TRUE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC004()
(* Both Conditions FALSE *)
BEGIN
    current_test := 'TC004: Both Conditions FALSE';
    test_newVar1 := 10;  (* 10 >= 12 → FALSE *)
    test_newVar2 := 15;  (* 15 <= 12 → FALSE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

(*
 * TEST CATEGORY 2: Boundary Value Analysis
 *)

PROCEDURE TC005()
(* newVar1 At Threshold (12) *)
BEGIN
    current_test := 'TC005: newVar1 Equals Threshold (12)';
    test_newVar1 := 12;  (* 12 >= 12 → TRUE *)
    test_newVar2 := 10;  (* 10 <= 12 → TRUE *)
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC006()
(* newVar1 Just Below Threshold (11) *)
BEGIN
    current_test := 'TC006: newVar1 Just Below Threshold (11)';
    test_newVar1 := 11;  (* 11 >= 12 → FALSE *)
    test_newVar2 := 10;  (* 10 <= 12 → TRUE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC007()
(* newVar1 Just Above Threshold (13) *)
BEGIN
    current_test := 'TC007: newVar1 Just Above Threshold (13)';
    test_newVar1 := 13;  (* 13 >= 12 → TRUE *)
    test_newVar2 := 10;  (* 10 <= 12 → TRUE *)
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC008()
(* newVar2 At Threshold (12) *)
BEGIN
    current_test := 'TC008: newVar2 Equals Threshold (12)';
    test_newVar1 := 15;  (* 15 >= 12 → TRUE *)
    test_newVar2 := 12;  (* 12 <= 12 → TRUE *)
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC009()
(* newVar2 Just Below Threshold (11) *)
BEGIN
    current_test := 'TC009: newVar2 Just Below Threshold (11)';
    test_newVar1 := 15;  (* 15 >= 12 → TRUE *)
    test_newVar2 := 11;  (* 11 <= 12 → TRUE *)
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC010()
(* newVar2 Just Above Threshold (13) *)
BEGIN
    current_test := 'TC010: newVar2 Just Above Threshold (13)';
    test_newVar1 := 15;  (* 15 >= 12 → TRUE *)
    test_newVar2 := 13;  (* 13 <= 12 → FALSE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

(*
 * TEST CATEGORY 3: Edge Cases
 *)

PROCEDURE TC011()
(* Minimum DINT Value *)
BEGIN
    current_test := 'TC011: Minimum DINT Value';
    test_newVar1 := -2147483648;  (* < 12 → FALSE *)
    test_newVar2 := -2147483648;  (* < 12 → TRUE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC012()
(* Maximum DINT Value *)
BEGIN
    current_test := 'TC012: Maximum DINT Value';
    test_newVar1 := 2147483647;   (* >= 12 → TRUE *)
    test_newVar2 := 2147483647;   (* > 12 → FALSE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC013()
(* Zero Values *)
BEGIN
    current_test := 'TC013: Zero Input Values';
    test_newVar1 := 0;  (* 0 >= 12 → FALSE *)
    test_newVar2 := 0;  (* 0 <= 12 → TRUE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC014()
(* Negative Values *)
BEGIN
    current_test := 'TC014: Negative Input Values';
    test_newVar1 := -50;   (* -50 >= 12 → FALSE *)
    test_newVar2 := -100;  (* -100 <= 12 → TRUE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC015()
(* Large Magnitude Values *)
BEGIN
    current_test := 'TC015: Large Magnitude Values';
    test_newVar1 := 1000000;  (* >= 12 → TRUE *)
    test_newVar2 := 999999;   (* > 12 → FALSE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

(*
 * TEST CATEGORY 4: Output Validation
 *)

PROCEDURE TC016()
(* Output = 777 when TRUE *)
BEGIN
    current_test := 'TC016: Output = 777 (Condition TRUE)';
    test_newVar1 := 20;  (* >= 12 → TRUE *)
    test_newVar2 := 5;   (* <= 12 → TRUE *)
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC017()
(* Output = 44 when FALSE *)
BEGIN
    current_test := 'TC017: Output = 44 (Condition FALSE)';
    test_newVar1 := 5;   (* < 12 → FALSE *)
    test_newVar2 := 20;  (* > 12 → FALSE *)
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
END_PROCEDURE;

PROCEDURE TC018()
(* Boolean Flag Correctness *)
BEGIN
    current_test := 'TC018: Boolean Flag Correctness';
    test_newVar1 := 20;  (* >= 12 → TRUE *)
    test_newVar2 := 8;   (* <= 12 → TRUE *)
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
END_PROCEDURE;

(*
 * TEST CATEGORY 5: Consistency Tests
 *)

PROCEDURE TC019()
(* Repeated Execution Consistency - Execute 10 times *)
VAR
    i : INT;
    all_pass : BOOL := TRUE;
END_VAR
BEGIN
    current_test := 'TC019: Repeated Execution Consistency';
    test_newVar1 := 15;
    test_newVar2 := 10;
    expected_outVar := 777;
    expected_x := TRUE;
    
    FOR i := 1 TO 10 DO
        RunTest();
        IF NOT test_passed THEN
            all_pass := FALSE;
        END_IF;
    END_FOR;
    
END_PROCEDURE;

PROCEDURE TC020()
(* State Independence *)
VAR
    test1_pass : BOOL;
    test2_pass : BOOL;
    test3_pass : BOOL;
END_VAR
BEGIN
    current_test := 'TC020: State Independence';
    
    (* First execution *)
    test_newVar1 := 15;
    test_newVar2 := 10;
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
    test1_pass := test_passed;
    
    (* Different execution *)
    test_newVar1 := 5;
    test_newVar2 := 20;
    expected_outVar := 44;
    expected_x := FALSE;
    RunTest();
    test2_pass := test_passed;
    
    (* Back to first values - should match *)
    test_newVar1 := 15;
    test_newVar2 := 10;
    expected_outVar := 777;
    expected_x := TRUE;
    RunTest();
    test3_pass := test_passed;
    
    (* Verify consistency: test1 and test3 should both pass *)
    IF NOT (test1_pass AND test3_pass) THEN
        fail_count := fail_count + 1;
    END_IF;
    
END_PROCEDURE;

(*
 ============================================
 MAIN TEST EXECUTION
 ============================================
 *)

(* Initialize test counters *)
test_count := 0;
pass_count := 0;
fail_count := 0;

(* Execute all test cases *)
TC001();  (* TC001-TC010: 10 tests *)
TC002();
TC003();
TC004();
TC005();
TC006();
TC007();
TC008();
TC009();
TC010();

TC011();  (* TC011-TC015: 5 tests *)
TC012();
TC013();
TC014();
TC015();

TC016();  (* TC016-TC018: 3 tests *)
TC017();
TC018();

TC019();  (* TC019-TC020: 2 tests *)
TC020();

(* Test execution complete *)

END_PROGRAM
