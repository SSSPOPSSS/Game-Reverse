#include <iostream>
using namespace std;
const int MAX = 100;
int HexArr[MAX] = { 0 };

int inthex(int aDec)			// 十进制转十六进制并输出
{
	int temp = 0;
	int count = 0;

	while (0 != aDec)			// aDec除16取余，直至商为零
	{
		temp = aDec;
		aDec /= 16;
		HexArr[count++] = (temp %= 16);	// 后置++，执行这条语句完后再进行count++
	}

	for (int i = count - 1; i >= 0; i--)	// 倒序输出
	{
		if (HexArr[i] > 9)
		{
			printf("%c", HexArr[i] - 10 + 'A');
		}
		else
		{
			printf("%d", HexArr[i]);
		}
	}
	printf("\n");
	return 0;
}

int hexint(char *hex)			// 十六进制转十进制
{
	int value = 0;
	while (*hex)
	{
		if (*hex >= 'A' && *hex <= 'F')
			value = (*hex - 55) + 16 * value;
		else if (*hex >= 'a' && *hex <= 'f')
			value = (*hex - 87) + 16 * value;
		else if (*hex >= '0' && *hex <= '9')
			value = (*hex - 48) + 16 * value;
		else
		{
			return value;
		}
		hex++;
	}
	return value;
}

int main()
{
	char op;
	char hex[360];
	int ints;
	
	printf("欢迎使用十六进制和十进制转换工具 作者:凉笙\n\n");

	cout << "1.十六进制转换十进制\n2.十进制转换十六进制\n\n请选择算法(1/2):";
	cin >> op;

	switch (op)
	{
	case '1':
		cout << "\n已进入十六进制转换\n输入十六进制数: ";
		cin >> hex;
		printf("\n结果:%d", hexint(hex));
		break;

	case '2':
		cout << "\n已进入十进制转换\n输入十进制数: ";
		cin >> ints;
		printf("\n结果:");
		inthex(ints);
		break;

	default:
		cout << "Error!  请输入正确算法";
		break;
	}

	return 0;
}
