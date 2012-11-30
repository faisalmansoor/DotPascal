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
 .method public hidebysig static int32 FACTORIAL(int32 N) cil managed 
 { 
 .locals init (int32 V_0,int32 V_1) 
  //Start IF  
  ldarg.0 
  ldc.i4.s 1 
  bgt.s  L3 
  ldc.i4.s 1 
  br  L2 
  L3: 
  ldc.i4.s 0 
  L2: 
  brfalse	L0 
  //True  
  ldc.i4.s 1 
  stloc.1 
  br	L1 
  L0: 
  //ELSE  
  ldarg.0 
  ldarg.0 
  ldc.i4.s 1 
  sub 
  call  int32 PROCTEST::FACTORIAL(int32 ) 
  mul 
  stloc.1 
  L1: 
  //Enf IF  
  ldloc.s	V_1 
  ret
 } 
 .method public hidebysig static void  Main() cil managed 
 { 
 .entrypoint
 
  ldstr    "Enter any number to caluculate its factorial :" 
  call       void [mscorlib]System.Console::Write(string) 
  call  int32 PROCTEST::READ() 
  stsfld     int32 PROCTEST::I 
  ldstr    "Factorial of " 
  call       void [mscorlib]System.Console::Write(string) 
  ldsfld     int32 PROCTEST::I 
  call       void [mscorlib]System.Console::Write(int32) 
  ldstr    " = " 
  call       void [mscorlib]System.Console::Write(string) 
  ldsfld     int32 PROCTEST::I 
  call  int32 PROCTEST::FACTORIAL(int32 ) 
  call       void [mscorlib]System.Console::Write(int32) 
  ret 
 } 
} 
