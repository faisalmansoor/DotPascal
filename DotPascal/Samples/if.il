.assembly DotPascal{} 
.class PROCTEST
{ 
 .field private static int32 I 
 .method public hidebysig specialname rtspecialname  
 instance void  .ctor() cil managed 
 { 
 .maxstack  1 
 ldarg.0 
 call       instance void [mscorlib]System.Object::.ctor() 
 ret 
 } // end of method .ctor 
 .method private hidebysig specialname rtspecialname static  
 void  .cctor() cil managed 
 { 
 ret 
 } // end of method .cctor 
 .method public hidebysig static int32  READ() cil managed  
 { 
 call       string [mscorlib]System.Console::ReadLine() 
 call       int32 [mscorlib]System.Int32::Parse(string) 
 ret 
 }// end of method ReadInt 
 .method public hidebysig static int32  READINT() cil managed  
 { 
 call       string [mscorlib]System.Console::ReadLine() 
 call       int32 [mscorlib]System.Int32::Parse(string) 
 ret 
 }// end of method ReadInt 
 .method public hidebysig static float64  READREAL() cil managed 
 { 
 call       string [mscorlib]System.Console::ReadLine() 
 call       float64 [mscorlib]System.Double::Parse(string) 
 ret 
 } // end of method ReadReal 
 .method public hidebysig static void  Main() cil managed 
 { 
 .entrypoint
 
  ldstr    "Enter any integer: " 
  call       void [mscorlib]System.Console::Write(string) 
  call  int32 PROCTEST::READINT() 
  stsfld     int32 PROCTEST::I 
  //Start IF  
  ldsfld     int32 PROCTEST::I 
  ldc.i4.s 10 
  cgt 
  brfalse	L0 
  //True  
  ldstr    "You entered " 
  call       void [mscorlib]System.Console::Write(string) 
  ldsfld     int32 PROCTEST::I 
  call       void [mscorlib]System.Console::Write(int32) 
  ldstr    " which is greater than 10" 
  call       void [mscorlib]System.Console::Write(string) 
  br	L1 
  L0: 
  //ELSE  
  ldstr    "You entered " 
  call       void [mscorlib]System.Console::Write(string) 
  ldsfld     int32 PROCTEST::I 
  call       void [mscorlib]System.Console::Write(int32) 
  ldstr    " which is less than 10" 
  call       void [mscorlib]System.Console::Write(string) 
  L1: 
  //Enf IF  
  ret 
 } 
} 
