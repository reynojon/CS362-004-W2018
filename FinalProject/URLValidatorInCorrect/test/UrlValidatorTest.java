

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
	   UrlValidator urlVal = new UrlValidator(null, null, UrlValidator.ALLOW_ALL_SCHEMES);
	   boolean expectedResult, testResult;
	   int randomURLLength, randNumber1, randNumber2;
	   String randomURL;
	   
	   //SOURCE: https://www.javamex.com/tutorials/random_numbers/seeding.shtml
	   Random rand = new Random(System.nanoTime());
	   
	   //Sets of valid and invalid values for each partition of the domain
	   
	   String[] validScheme = {"http://", "ftp://", "https://", ""};
	   String[] invalidScheme = {"htt://", "http:/", "https:/", ".", "abc://", ":", "://", ":80"};
	   
	   String[] validSite = {"www.google.com", "google.com", "www.oregonstate.edu", "access.engr.oregonstate.edu", "home.bing.com", "192.168.1.1", "255.255.0.0"};
	   String[] invalidSite = {".google.com", "google.", "www", "www.", "192.168.1", "256.168.1.1", "168.-1.1.1", "www.1", "bing.abcd", "", ":"};
	   
	   String[] validPort = {"", ":80", ":65535", ":0", ":65534", ":1", ":6555", ":655"};
	   String[] invalidPort = {":.", ":a", ":-1", ":65536", "12345"};
	   
	   String[] validPath = {"/", "", "/abra", "/abra/", "/A//", "/image.png", "/ashsadawqlhg/aihfl/qwh/as/ahhig/ashi"};
	   String[] invalidPath = {"\\", "\\\\", ".", "words with space", " ", ".asp", "//"};
	   
	   

	   for(int i = 0; i < 500; i++)
	   {
		   randomURL = "";
		   expectedResult = true;
		   
		   //Determine length of piece to test
		   randomURLLength = rand.nextInt(2) + 3;	//Random number between 3 and 4
		   
		   randNumber1 = rand.nextInt(2); //0 or 1
		   if (randNumber1 == 0)	//URL will begin with http etc.
		   {
			   randNumber2 = rand.nextInt(10); // 0 to 9
			   if(randNumber2 != 0)	//90% chance to choose a valid part for the URL
			   {
				   randomURL += validScheme[rand.nextInt(validScheme.length)];	//Random element in valid scheme array
			   }
			   else
			   {
				   randomURL += invalidScheme[rand.nextInt(invalidScheme.length)];  //Random element in invalid scheme array
				   expectedResult = false;
			   }
		   }
			
		   
		   randNumber2 = rand.nextInt(10); // 0 to 9
		   if(randNumber2 != 0)	//90% chance to choose a valid part for the URL
		   {
			   randomURL += validSite[rand.nextInt(validSite.length)];	//Random element in valid site array
		   }
		   else
		   {
			   randomURL += invalidSite[rand.nextInt(invalidSite.length)];  //Random element in invalid site array
			   expectedResult = false;
		   }
		   
		   randNumber2 = rand.nextInt(10); // 0 to 9
		   if(randNumber2 != 0)	//90% chance to choose a valid part for the URL
		   {
			   randomURL += validPort[rand.nextInt(validPort.length)];	//Random element in valid port array
		   }
		   else
		   {
			   randomURL += invalidPort[rand.nextInt(invalidPort.length)];  //Random element in invalid port array
			   expectedResult = false;
		   }
		   
		   randNumber2 = rand.nextInt(10); // 0 to 9
		   if(randNumber2 != 0)	//90% chance to choose a valid part for the URL
		   {
			   randomURL += validPath[rand.nextInt(validPath.length)];	//Random element valid path in array
		   }
		   else
		   {
			   randomURL += invalidPath[rand.nextInt(invalidPath.length)];  //Random element in invalid path array
			   expectedResult = false;
		   }
		   
		   System.out.print(randomURL);
		   System.out.print(" - Expected: ");
		   System.out.print(expectedResult);
		   System.out.print(" - Test: ");
		   
		   testResult = urlVal.isValid(randomURL);
		   System.out.print(testResult);
	
		   if(expectedResult == testResult)
		   {
			   System.out.println(" - TEST PASSED");
		   }
		   else
		   {
			   System.out.println(" - TEST FAILED");
		   }
	   }
   }
   


}
