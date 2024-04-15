# Compatability

### Load/Store Operations:

| LDA          | Load Accumulator |     |
| ------------ | ---------------- | --- |
| Immediate    | INS_LDA_IM       | $A9 |
| Zero Page    | INS_LDA_ZP       | $A5 |
| Zero Page X  | INS_LDA_ZP_X     | $B5 |
| Absolute     | INS_LDA_ABS      | $AD |
| Absolute X   | INS_LDA_ABS_X    | $BD |
| (Indirect) X | INS_LDA_IND_X    | $A1 |


### Jumps & Calls

| JMP      | Jump T another Location |     |
| -------- | ----------------------- | --- |
| Absolute | INS_JMP_ABS             | $4C |
| Indirect | INS_JMP_IND             | $6C |

| JSR      | Jump to SubRoutine |     |
| -------- | ------------------ | --- |
| Absolute | INS_JSR            | $20 |

