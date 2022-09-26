void showclock(char timec[], bool flag) //flag useless
{
    if (flag == HIGH)
    {
        display.fillRect(7, 20, 125, 28, BLACK);
        display.setTextColor(WHITE);
        display.setTextSize(4);
        display.setCursor(7, 20);
        display.println(timec);
        display.display();
    }
    else
    {
        display.fillRect(7, 20, 125, 28, BLACK);
        display.display();
    }
}