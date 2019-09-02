import java.lang.Character;
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Arrays;


public class Test{
   

	public static void main(String[] args){
        
        File file = new File("front.in");
        FileReader reader = null;

        try {
            reader = new FileReader(file);
        } catch (Exception e) {
            e.printStackTrace();
        }

        //System.out.println(String.valueOf((char)reader.read()));

        /*char[] buffer = new char[4];
        reader.read(buffer);
        System.out.println(String.valueOf(buffer));


        int end;
        while((end = reader.read(buffer))!=-1){
            System.out.println(String.valueOf(buffer));
        }
        reader.close();*/

        //char str[] = {'H','e','l','l','o','W','o','r','l','d'};
        char str[] = new char[100];
        str[0] = 'H';
        str[1] = 'e';
        str[2] = 'l';
        str[3] = 'l';
        str[4] = 'o';
        str[5] = 'W';
        str[6] = 'o';
        str[7] = 'r';
        str[8] = 'l';
        str[9] = 'd';
        str[10] = '\0';

        /*int singleCh = 0;
            while((singleCh = fileReader.read()) != -1){
                System.out.print((char)singleCh);
            }
            fileReader.close();*/

            String str1 = new String(str);
            String str2 = new String("HelloWorld");
            String str3 = "";
            str3 += '!';
            str3 += '!';
            /*System.out.println(str1.length());
            System.out.println("for".length());
            System.out.println("HelloWorld".length());
            System.out.println(str1.equals(str2));
            System.out.println(str1.compareTo(str2));*/
            System.out.println(str3);
    }

}
