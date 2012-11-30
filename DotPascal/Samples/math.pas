program procTest;  
  var i:integer;
  function sum(a,b:integer):integer;  
  begin
  	sum := a + b;
  end
  function diff(a,b:integer):integer;
  begin
  	diff := a - b;
  end
  function div(a,b:integer):real;
  begin
  	div := a / b;
  end
  begin
    Write('\n10 / 3 = ',div(10));      
  end.
