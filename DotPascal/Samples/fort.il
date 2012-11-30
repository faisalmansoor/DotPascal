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
 
  //FOR START  
  //INITIALIZING LOOP VARIABLE  
  ldc.i4.s 1 
  stsfld     int32 PROCTEST::I 
  //LOOP VARIABLE INITIALIZED  
  //LOOP CONDITION YAHAN HAY  
  L0: 
  ldsfld     int32 PROCTEST::I 
  ldc.i4.s 10 
  bgt.s  L3 
  ldc.i4.s 1 
  br  L2 
  L3: 
  ldc.i4.s 0 
  L2: 
  //IF TRUE THEN CONTINUE ELSE JUMP END FOR  
  brfalse	L1 
  //This is Step Expression  
  ldc.i4.s 2 
  //Step Expression End  
   ldstr    "loop for time " 
   call       void [mscorlib]System.Console::Write(string) 
   ldsfld     int32 PROCTEST::I 
   call       void [mscorlib]System.Console::Write(int32) 
   ldstr    "\n" 
   call       void [mscorlib]System.Console::Write(string) 
  //INC LOOP VARIABLE  
  ldsfld     int32 PROCTEST::I 
  add 
  stsfld     int32 PROCTEST::I 
  //JUMP TO START FOR CONDITION  
  br	L0 
  //FOR FINISHED FOR GOOD  
  L1: 
  //FOR LOOP END HERE SHUKAR ALLAH KA  
  ret 
 } 
} 
