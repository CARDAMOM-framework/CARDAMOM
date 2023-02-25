function B=nan2zero(A);
B=A;
B(isnan(A))=0;