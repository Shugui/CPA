function C = chol_bloques( A, b )
% C = chol_bloques( A, b )
% Devuelve el factor de Cholesky de la matriz
% simétrica definida positiva A.
% Solo se referencia la parte triangular inferior de A.
% b es el tamaño de bloque.
%
% Pedro Alonso
% Julio 2013

n = size(A,1);
if( size(A,2) ~= n ) 
  error('La matriz ha de ser cuadrada');
end 
if( b > n ) 
  error('El tamaño de bloque ha de ser menor que n');
end 


  C = tril(A);
  for  k = 1:b:n
    mk = min(k+b-1,n);
    %D = C(k:mk,k:mk)+tril(C(k:mk,k:mk),-1)';
    %C(k:mk,k:mk) = chol(D,'lower');                 % cholesky
    C(k:mk,k:mk) = chol_escalar(C(k:mk,k:mk));
    for i=k+b:b:n
      mi = min(i+b-1,n);
      C(i:mi,k:mk) = C(i:mi,k:mk) / C(k:mk,k:mk)';  % trsm
    end
    for i=k+b:b:n
      mi = min(i+b-1,n);
      for j=k+b:b:i-1
        mj = min(j+b-1,n);
        C(i:mi,j:mj) = C (i:mi,j:mj) - C (i:mi,k:mk) * C (j:mj,k:mk)';  % gemm
      end
      D = C(i:mi,i:mi) + tril(C(i:mi,i:mi),-1)'; % syrk
      D = D - C(i:mi,k:mk) * C(i:mi,k:mk)';
      C(i:mi,i:mi) = tril(D);
    end
  end 

