package homework;

import notebook.Home;


public class HomeWork {


    public static void main(String[] args) {

        Home comp = new Home();
        
        
        comp.setEncapsulation("Материнская плата, видеокарта, процессор, динамики, вебкамера, микросхемы"); // Что инкапсулировано
        
        comp.setOutside("Рабочий стол, ярлыки (удаление, перемещение ярлыков), настройки, usb"); // Что выставлено наружу
        
        comp.setAmount(8);

        
        
        comp.open();
        comp.close();
        
    }
    
    
}
