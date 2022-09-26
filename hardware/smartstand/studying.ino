void studying(bool flag, char times[])
{
    if (flag == LOW)
    {
        display.fillRect(0, 56, 128, 5, BLACK);
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(24, 56);
        display.println("Start Focusing!!");
        display.display();
    }
    else
    {
        display.fillRect(0, 56, 128, 8, BLACK);
        display.setTextColor(WHITE);
        display.setTextSize(1);
        display.setCursor(51, 56);
        display.println(times);
        display.display();
    }
}