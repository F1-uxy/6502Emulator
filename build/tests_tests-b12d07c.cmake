add_test( [==[Memory Initialisation]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[Memory Initialisation]==]  )
set_tests_properties( [==[Memory Initialisation]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[CPU Register Reset]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[CPU Register Reset]==]  )
set_tests_properties( [==[CPU Register Reset]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[LDA_IM Load value into ACC Immediate]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[LDA_IM Load value into ACC Immediate]==]  )
set_tests_properties( [==[LDA_IM Load value into ACC Immediate]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[LDA_ZP can Load Value into Acc]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[LDA_ZP can Load Value into Acc]==]  )
set_tests_properties( [==[LDA_ZP can Load Value into Acc]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[LDA_ZP_X can Load Value into Acc with offset X]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[LDA_ZP_X can Load Value into Acc with offset X]==]  )
set_tests_properties( [==[LDA_ZP_X can Load Value into Acc with offset X]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[LDA_ABS can Load ]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[LDA_ABS can Load ]==]  )
set_tests_properties( [==[LDA_ABS can Load ]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[LDA_ABS_X can Load address offset by x]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[LDA_ABS_X can Load address offset by x]==]  )
set_tests_properties( [==[LDA_ABS_X can Load address offset by x]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[INS_JMP_ABS can change PC]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[INS_JMP_ABS can change PC]==]  )
set_tests_properties( [==[INS_JMP_ABS can change PC]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[INS_JMP_IND can change PC taking a 16 bit address that points to lower 8 bits of another address]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[INS_JMP_IND can change PC taking a 16 bit address that points to lower 8 bits of another address]==]  )
set_tests_properties( [==[INS_JMP_IND can change PC taking a 16 bit address that points to lower 8 bits of another address]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
add_test( [==[INS_JSR can push address to stack and set PC to target]==] C:/Users/Fluxy/Documents/Code/6502Emulator/build/Debug/tests.exe [==[INS_JSR can push address to stack and set PC to target]==]  )
set_tests_properties( [==[INS_JSR can push address to stack and set PC to target]==] PROPERTIES WORKING_DIRECTORY C:/Users/Fluxy/Documents/Code/6502Emulator/build)
set( tests_TESTS [==[Memory Initialisation]==] [==[CPU Register Reset]==] [==[LDA_IM Load value into ACC Immediate]==] [==[LDA_ZP can Load Value into Acc]==] [==[LDA_ZP_X can Load Value into Acc with offset X]==] [==[LDA_ABS can Load ]==] [==[LDA_ABS_X can Load address offset by x]==] [==[INS_JMP_ABS can change PC]==] [==[INS_JMP_IND can change PC taking a 16 bit address that points to lower 8 bits of another address]==] [==[INS_JSR can push address to stack and set PC to target]==])
