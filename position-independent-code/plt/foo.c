
int myglob = 10;

void func2()
{
	int a = 2;
}

int func(int a)
{
	myglob += a;
	
	func2();
	return myglob;

}
