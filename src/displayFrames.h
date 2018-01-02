
void oledUpdate() {

//Draw OLED screen and write data

        u8g2.firstPage();
        do {


// Write Txt in small font
                u8g2.setFont(u8g2_font_6x10_tf);
                u8g2.drawStr( 1, 9, "W'shop Shed  Outside");
//  u8g.drawStr( 48, 38, "Max");
//  u8g.drawStr( 48, 48, "Min");
                u8g2.drawStr( 48, 58, "<Set");

// Set large font and write main temps
                //u8g2.setFont(u8g_font_freedoomr10r);
                u8g2.setCursor(1, 25);
                u8g2.print(probe1);
                u8g2.setCursor(40, 25);
                u8g2.print(probe2);
                u8g2.setCursor(80, 25);
                u8g2.print(probe3);


//Set small font and write Max/Min's
                u8g2.setFont(u8g_font_6x12);

                u8g2.setCursor(1, 38);
                u8g2.print(maxTemp1);
                u8g2.setCursor(1, 48);
                u8g2.print(minTemp1);

                u8g2.setCursor(40, 38);
                u8g2.print(maxTemp2);
                u8g2.setCursor(40, 48);
                u8g2.print(minTemp2);


                u8g2.setCursor(80, 38);
                u8g2.print(maxTemp3);
                u8g2.setCursor(80, 48);
                u8g2.print(minTemp3);



                u8g2.setCursor(1, 58);
                u8g2.print(setTemp);




        } while( u8g2.nextPage() );


}
