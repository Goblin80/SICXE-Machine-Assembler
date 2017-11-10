short strInt(char a[])
{
    short r = 0;
    //for(int i = 0; a[i] != '\0'; r = 10 * r + a[i++] - '0');
    sscanf(a, "%hd", &r);
    return r;
}

short strReplace(char a[], char x)
{
    for(int i = 0; a[i] != '\0'; i++)
        if(a[i] == x)
            return !(a[i] = '\0');        
    return 0;
}

char* strCapitalize(char a[])
{
    for(int i = 0; a[i] != '\0'; i++)
        a[i] -= a[i] > 96 && a[i] < 123 ? 32 : 0;
    return a;
}
