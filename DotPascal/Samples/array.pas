program procTest;    
  var g:array[1..3,1..2,6..7,3..5] of real;
  procedure proc(q:array[1..3,1..2,6..7,3..5] of integer);
  begin
    
  end
  
  begin  
   proc( g );
   g[3,1,6,4] := 9.1;   
   Write(g[3,1,6,2]);
  end.
