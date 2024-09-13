# Compatability

### Load/Store Operations:

| LDA          | Load Accumulator |     | Tested? |
| ------------ | ---------------- | --- | ------- |
| Immediate    | LDA_IM           | $A9 | T       |
| Zero Page    | LDA_ZP           | $A5 | T       |
| Zero Page X  | LDA_ZP_X         | $B5 | T       |
| Absolute     | LDA_ABS          | $AD | T       |
| Absolute X   | LDA_ABS_X        | $BD | T       |
| (Indirect) X | LDA_IND_X        | $A1 | T       |

| LDX         | Load X Register |     |
| ----------- | --------------- | --- |
| Immediate   | LDX_IM          | $A2 |
| Zero Page   | LDX_ZP          | $A6 |
| Zero Page Y | LDX_ZP_Y        | $B6 |
| Absolute    | LDX_ABS         | $AE |
| Absolute Y  | LDX_ABS_Y       | $BE |

### Transfer Operations:

| TAX     | Transfer Acc to X |     |
| ------- | ----------------- | --- |
| Implied | TAX               | $AA |

| TAY     | Transfer Acc to Y |     |
| ------- | ----------------- | --- |
| Implied | TAY               | $A8 |

| TSX     | Transfer SP to X |     |
| ------- | ---------------- | --- |
| Implied | TSX              | $BA |

| TXA     | Transfer X to Acc |     |
| ------- | ----------------- | --- |
| Implied | TXA               | $8A |

| TXS     | Transfer X to SP |     |
| ------- | ---------------- | --- |
| Implied | TXS              | $9A |

| TYA     | Transfer Y to Acc |     |
| ------- | ----------------- | --- |
| Implied | TYA               | $98 |



### Jumps & Calls

| JMP      | Jump T another Location |     | Tested? |
| -------- | ----------------------- | --- | ------- |
| Absolute | JMP_ABS                 | $4C | T       |
| Indirect | JMP_IND                 | $6C | T       |

| JSR      | Jump to SubRoutine |     | Tested? |
| -------- | ------------------ | --- | ------- |
| Absolute | JSR                | $20 | T       |

| RTS     | Return from subreoutine |     | Tested? |
| ------- | ----------------------- | --- | ------- |
| Implied | RTS                     | $60 | F       |



### Logical Operations:

| AND          | Bitwise AND |     | Tested? |
| ------------ | ----------- | --- | ------- |
| Immediate    | AND_IM      | $29 | T       |
| Zero Page    | AND_ZP      | $25 | T       |
| Zero Page X  | AND_ZP_X    | $35 | T       |
| Absolute     | AND_ABS     | $2D | T       |
| Absolute X   | AND_ABS_X   | $3D | T       |
| Absolute Y   | AND_ABS_Y   | $39 | T       |
| (Indirect) X | AND_IND_X   | $21 | T       |
| (Indirect) Y | AND_IND_Y   | $31 | T       |

| XOR          | Bitwise XOR |     | Tested? |
| ------------ | ----------- | --- | ------- |
| Immediate    | XOR_IM      | $49 | F       |
| Zero Page    | XOR_ZP      | $45 | F       |
| Zero Page X  | XOR_ZP_X    | $55 | F       |
| Absolute     | XOR_ABS     | $4D | F       |
| Absolute X   | XOR_ABS_X   | $5D | F       |
| Absolute Y   | XOR_ABS_Y   | $59 | F       |
| (Indirect) X | XOR_IND_X   | $41 | F       |
| (Indirect) Y | XOR_IND_Y   | $51 | F       |

| IOR          | Bitwise IOR |     | Tested? |
| ------------ | ----------- | --- | ------- |
| Immediate    | IOR_IM      | $09 | F       |
| Zero Page    | IOR_ZP      | $05 | F       |
| Zero Page X  | IOR_ZP_X    | $15 | F       |
| Absolute     | IOR_ABS     | $0D | F       |
| Absolute X   | IOR_ABS_X   | $1D | F       |
| Absolute Y   | IOR_ABS_Y   | $19 | F       |
| (Indirect) X | IOR_IND_X   | $01 | F       |
| (Indirect) Y | IOR_IND_Y   | $11 | F       |

| BIT       | Bit Test |     | Tested? |
| --------- | -------- | --- | ------- |
| Zero Page | BIT_ZP   | $24 | F       |
| Absolute  | BIT_ABS  | $2C | F       |


### Stack Operations:

| PHA     | Push Acc on stack |     | Tested? |
| ------- | ----------------- | --- | ------- |
| Implied | PHA               | $48 | F       |

| PHP     | Push status on stack |     | Tested? |
| ------- | -------------------- | --- | ------- |
| Implied | PHP                  | $08 | F       |

| PLP     | Pull status from stack |     | Tested? |
| ------- | ---------------------- | --- | ------- |
| Implied | PLP                    | $28 | F       |

| PLA     | Pull Acc from stack |     | Tested? |
| ------- | ------------------- | --- | ------- |
| Implied | PLA                 | $68 | F       |
