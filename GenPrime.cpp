void GenPrime(ZZ& n, long k)
{
    int err = 512;
   if (k <= 1) LogicError("GenPrime: bad length");

   if (k > (1L << 20)) ResourceError("GenPrime: length too large");

   if (err < 1) err = 1;
   if (err > 512) err = 512;

   if (k == 2) {
      if (RandomBnd(2))
         n = 3;
      else
         n = 2;

      return;
   }


   long t;

   t = 1;
   while (!ErrBoundTest(k, t, err))
      t++;

   RandomPrime(n, k, t);
}