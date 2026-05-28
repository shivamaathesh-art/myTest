# mFBD Function Block - Technical Documentation

## Overview

**Function Block Name:** `myFBD`  
**Type:** Function Block (FB)  
**Format:** PLCopen XML (IEC 61131-3)  
**IDE:** CODESYS V3.5 SP21  
**Created:** 2026-05-28

---

## Function Block Description

### Purpose
The `myFBD` function block implements a conditional selection logic that:
- Compares two input values against a threshold (tmp = 12)
- Combines the comparison results using logical AND
- Selects an output value based on the combined condition

### Functionality
1. **Input newVar1** compared with threshold 12 using **GE (>=)** operator
2. **Input newVar2** compared with threshold 12 using **LE (<=)** operator
3. Both results combined with **AND** operator
4. Result determines output selection via **SEL** multiplexer

---

## Interface Definition

### Input Variables

| Name | Type | Range | Description |
|------|------|-------|-------------|
| newVar1 | DINT | -2,147,483,648 to 2,147,483,647 | First comparison value |
| newVar2 | DINT | -2,147,483,648 to 2,147,483,647 | Second comparison value |

### Output Variables

| Name | Type | Values | Description |
|------|------|--------|-------------|
| outVar | DINT | 777 or 44 | Selected output based on condition |
| x | BOOL | TRUE/FALSE | Combined condition result |

---

## Logic Description

### Decision Logic

```
Condition1 = (newVar1 >= 12)
Condition2 = (newVar2 <= 12)
Combined = Condition1 AND Condition2

IF Combined THEN
    outVar = 777
ELSE
    outVar = 44
END_IF

x = Combined
```

### Block Diagram

```
newVar1 ────┬──→ [GE] ──┬─────────┐
            │           │         │
tmp(12)─────┘           │         ├──→ [AND] ──┬──→ x (Output)
                        │         │            │
newVar2 ────┬──→ [LE] ──┘         │     ┌──────┤
            │                     │     │      │
tmp(12)─────┘              777 ───┼─┐   │      │
                                  │ ├─→ [SEL] ──→ outVar
                           44 ────┘ │   │
                                    └───┘
```

---

## Truth Table

| newVar1 | newVar2 | GE(≥12) | LE(≤12) | AND | x | outVar |
|---------|---------|---------|---------|-----|---|--------|
| 10      | 10      | FALSE   | TRUE    | FALSE | FALSE | 44 |
| 10      | 15      | FALSE   | FALSE   | FALSE | FALSE | 44 |
| 15      | 10      | TRUE    | TRUE    | TRUE | TRUE | 777 |
| 15      | 15      | TRUE    | FALSE   | FALSE | FALSE | 44 |
| 12      | 12      | TRUE    | TRUE    | TRUE | TRUE | 777 |
| 13      | 11      | TRUE    | TRUE    | TRUE | TRUE | 777 |
| 20      | 20      | TRUE    | FALSE   | FALSE | FALSE | 44 |

---

## Equivalent Code

### Structured Text (ST)

```st
PROGRAM myFBD_ST
VAR
    newVar1 : DINT;
    newVar2 : DINT;
    outVar : DINT;
    x : BOOL;
    tmp : DINT := 12;
END_VAR

x := (newVar1 >= tmp) AND (newVar2 <= tmp);
IF x THEN
    outVar := 777;
ELSE
    outVar := 44;
END_IF;
END_PROGRAM
```

### C Code Equivalent

```c
typedef struct {
    int newVar1;
    int newVar2;
    int outVar;
    bool x;
} myFBD_t;

void myFBD_execute(myFBD_t *fb) {
    int tmp = 12;
    fb->x = (fb->newVar1 >= tmp) && (fb->newVar2 <= tmp);
    fb->outVar = fb->x ? 777 : 44;
}
```

---

## Performance

- **Execution Time:** < 1 µs (typical PLC)
- **Memory Usage:** ~22 bytes
- **Cycle Time:** Deterministic, no loops

---

## Use Cases

1. **Dual Sensor Validation** - Verify both sensors are in acceptable range
2. **Temperature Control** - Check if current and previous temps are acceptable
3. **Pressure Monitoring** - Validate dual pressure readings
4. **Multi-level Process Control** - Select operation based on dual conditions

---

## Document Revision

| Version | Date | Changes |
|---------|------|---------|
| 1.0 | 2026-05-28 | Initial documentation |
