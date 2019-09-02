import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
 
public class ReadText1 {

    public static void print() {
       printmethod();
    }

    public static void printmethod() {
        System.out.println("Hello World");
    }

    public static void main(String[] args){
        try{
            //파일 객체 생성
            File file = new File("front.in");
            //입력 스트림 생성
            FileReader filereader = new FileReader(file);
            int singleCh = 0;
            while((singleCh = filereader.read()) != -1){
                System.out.print((char)singleCh);
            }
            System.out.println();
            filereader.close();
        }catch (FileNotFoundException e) {
            // TODO: handle exception
        }catch(IOException e){
            System.out.println(e);
        }

        print();
    }
}
