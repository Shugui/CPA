function C = chol_escalar( A )
% C = chol_escalar( A )
% Devuelve el factor de Cholesky de la matriz
% simétrica definida positiva A.
% Este algoritmo solo referencia la parte triangular 
% inferior de la matriz.
% Algoritmo escalar.
%
% Pedro Alonso
% Julio 2013

n = size(A,1);
if( size(A,2) ~= n ) 
  error('La matriz ha de ser cuadrada');
end 

  % Este algoritmo procesa solo la parte triangular inferior de la matriz
  for  k = 1:n
    c = sqrt(A(k,k));
    A(k,k) = c;
    for i=k+1:n
      A(i,k) = A(i,k)/c;
    end
    for i=k+1:n
      for j=k+1:i-1
        A(i,j) = A(i,j) - A(i,k) * A(j,k);
      end
      A(i,i) = A(i,i) - A(i,k) * A(i,k);
    end
  end 
  C = tril(A);

