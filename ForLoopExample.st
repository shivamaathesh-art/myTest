PROGRAM ForLoopExample
(*
 * IEC 61131-3 Structured Text (ST) Program
 * Simple FOR Loop Example
 * 
 * This program demonstrates a basic FOR loop in IEC 61131-3 Structured Text syntax.
 * A FOR loop iterates a fixed number of times with an automatic counter variable.
 * 
 * Author: PLC Programmer
 * Date: 2026-05-28
 * Version: 1.0
 *)

VAR
    (* Loop counter variable - automatically managed by FOR loop *)
    i : INT;
    
    (* Array to store loop results *)
    result_array : ARRAY[1..10] OF INT;
    
    (* Simple counter variable *)
    counter : INT := 0;
    
    (* Total sum variable *)
    total_sum : INT := 0;

END_VAR

(* Main program logic *)

(* Example 1: Simple FOR loop counting from 1 to 10 *)
FOR i := 1 TO 10 DO
    (* Store the loop counter in array *)
    result_array[i] := i;
END_FOR;

(* Example 2: FOR loop with increment step *)
(* Count from 0 to 20 with step of 2 *)
counter := 0;
FOR i := 0 TO 20 BY 2 DO
    counter := counter + 1;
END_FOR;
(* After loop: counter = 11 *)

(* Example 3: FOR loop with calculations *)
total_sum := 0;
FOR i := 1 TO 10 DO
    (* Add each number from 1 to 10 *)
    total_sum := total_sum + i;
END_FOR;
(* After loop: total_sum = 55 (1+2+3+...+10) *)

(* Example 4: Nested FOR loops *)
FOR i := 1 TO 5 DO
    FOR counter := 1 TO 3 DO
        (* This block executes 5 * 3 = 15 times *)
        result_array[i] := result_array[i] + counter;
    END_FOR;
END_FOR;

(* Example 5: FOR loop with EXIT statement *)
counter := 0;
FOR i := 1 TO 100 DO
    counter := counter + 1;
    
    (* Exit loop early if counter reaches 25 *)
    IF counter >= 25 THEN
        EXIT;  (* Break out of loop *)
    END_IF;
END_FOR;
(* After loop: counter = 25 *)

(* Example 6: FOR loop with CONTINUE statement *)
total_sum := 0;
FOR i := 1 TO 10 DO
    (* Skip even numbers *)
    IF i MOD 2 = 0 THEN
        CONTINUE;  (* Skip to next iteration *)
    END_IF;
    
    (* This only executes for odd numbers *)
    total_sum := total_sum + i;
END_FOR;
(* After loop: total_sum = 25 (1+3+5+7+9) *)

END_PROGRAM
