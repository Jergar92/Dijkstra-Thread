#include <thread>
#include <iostream>
#include <mutex>
#include <vector>
#include <time.h>       /* time */
int menssage_avaible = -1;
int counter = 0;
int id = 0;

std::mutex mtx;
std::condition_variable cond;
std::vector<int> id_vec;
bool Find(int to_find)
{
	for (int i = 0; i < id_vec.size(); i++)
	{
		if (id_vec[i] == to_find)
			return true;
	}
	return false;
}

void increment(int iteration,int id)
{
	id_vec.push_back(id);
	int my_count =0;
	{
		std::unique_lock<std::mutex> lock(mtx);
		while (!Find(menssage_avaible))
		{
			cond.wait(lock);

		}
		for (int i = 0; i < iteration; i++)
		{
			counter++;
			my_count++;
		}
		std::cout << "my_count =" << my_count << std::endl;
	}

}

void send(int id)
{
	std::unique_lock<std::mutex> lock(mtx);
	menssage_avaible = id;
	cond.notify_one();
	
}
int main()
{
	srand(time(NULL));
	std::thread t1(increment, 100000,rand()%100);

	std::thread t2(increment, 100000, rand() % 100);

	for (int i = 0; i < id_vec.size(); i++)
	{
		send(id_vec[i]);

	}
	t1.join();
	t2.join();

	std::cout << "Counter =" << counter << std::endl;
	getchar();
	return 0;
}