

import junit.framework.TestCase;

//You can use this as a skeleton for your 3 different test approach
//It is an optional to use this file, you can generate your own test file(s) to test the target function!
// Again, it is up to you to use this file or not!


public class UrlValidatorTest extends TestCase {


   public UrlValidatorTest(String testName) {
      super(testName);
   }

   public void testManualTest()
   {
   //You can use this function to implement your manual testing
      UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);

      assertTrue(urlVal.isValid("http://google.com"));
      assertTrue(urlVal.isValid("http://ww2w.google.com"));
      assertTrue(urlVal.isValid("http://www.google.com/?q=hello"));
      assertTrue(urlVal.isValid("http://255.255.255.255"));

      // assertFalse(urlVal.isValid("htitp://www.google.com")); // regexvalidator.java line 120 != -> ==
      // assertFalse(urlVal.isValid("http:/www.google.com/"));
      // assertFalse(urlVal.isValid("http://www.google."));
      // assertFalse(urlVal.isValid("http://1.2.3.4."));
      // assertFalse(urlVal.isValid("http://.1.2.3.4"));
      // assertFalse(urlVal.isValid("http://256.256.256.256"));
      // assertFalse(urlVal.isValid("http://go.a1a"));
      assertFalse(urlVal.isValid("http://")); // it says http instead of file: in line 318 of urlValidator.java

   }

   public void testYourFirstPartition()
   {
	 //You can use this function to implement your First Partition testing

   }

   public void testYourSecondPartition(){
		 //You can use this function to implement your Second Partition testing

   }
   //You need to create more test cases for your Partitions if you need to

   public void testIsValid()
   {
	   //You can use this function for programming based testing

   }



}
