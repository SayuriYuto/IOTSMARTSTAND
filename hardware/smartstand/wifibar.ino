void wifibar(bool flag)
{
    if (flag == HIGH)
    {
        //  Serial.println("Yeswifibar");
        display.fillRect(120, 0, 7, 9, BLACK);
        display.drawRect(126, 1, 1, 8, WHITE);
        display.drawRect(124, 3, 1, 6, WHITE);
        display.drawRect(122, 5, 1, 4, WHITE);
        display.drawRect(120, 7, 1, 2, WHITE);
        display.display();
    }
    else
    {
        display.fillRect(120, 0, 7, 9, BLACK);
        display.drawRect(124, 1, 1, 6, WHITE);
        display.drawPixel(124, 8, WHITE);
        display.display();
    }
}