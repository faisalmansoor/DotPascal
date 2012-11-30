program procTest;    
  var i,j:array[1..3] of integer;
      k : integer;
      m : array[1..3] of integer;
      l :real;
  begin   
   k := 2;
   i[k] := 1;
   m[k + 1] := 2;
   k := i[2];   
   Write(k);
  end.
