void firebaseConnected(bool flag)
{
    if (flag == HIGH)
    {
        //  Serial.println("Firebase connected");
        display.fillRect(108, 1, 11, 8, BLACK);
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(108, 2);
        display.println("FB");
        display.display();
    }
    else
    {
        display.fillRect(108, 1, 11, 8, BLACK);
    }
}