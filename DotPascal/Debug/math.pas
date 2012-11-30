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
    Write('Nested Funtion Call :',sum(7,sum(1,sum(2,sum(sum(4,5),4)))));
    Write('\n10 + 2 = ',sum(10,2));
    Write('\n10 - 2 = ',diff(10,2));
    Write('\n10 / 2 = ',div(10,2)); 
    Write('\n10 / 3 = ',div(10,3));      
  end.
