.assembly DotPascal{} 
.class PROCTEST
{ 
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
