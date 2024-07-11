#include <iostream>
#include <vector>
#include <unordered_map>

template <typename T>
class Heap
{
public:
    std::vector<T> data;
    std::unordered_map<T, int> map;

    void add(T item)
    {
        data.push_back(item);
        map[item] = data.size() - 1;
        upheapify(data.size() - 1);
    }

    void display()
    {
        for (const auto& item : data)
        {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }

    int size() const
    {
        return data.size();
    }

    bool isEmpty() const
    {
        return data.empty();
    }

    T remove()
    {
        swap(0, data.size() - 1);
        T rv = data.back();
        data.pop_back();
        downheapify(0);
        map.erase(rv);
        return rv;
    }

    T get() const
    {
        return data[0];
    }

    void updatePriority(T pair)
    {
        int index = map[pair];
        upheapify(index);
    }

private:
    void upheapify(int ci)
    {
        int pi = (ci - 1) / 2;
        if (ci > 0 && isLarger(data[ci], data[pi]))
        {
            swap(pi, ci);
            upheapify(pi);
        }
    }

    void downheapify(int pi)
    {
        int lci = 2 * pi + 1;
        int rci = 2 * pi + 2;
        int mini = pi;

        if (lci < data.size() && isLarger(data[lci], data[mini]))
        {
            mini = lci;
        }

        if (rci < data.size() && isLarger(data[rci], data[mini]))
        {
            mini = rci;
        }

        if (mini != pi)
        {
            swap(mini, pi);
            downheapify(mini);
        }
    }

    void swap(int i, int j)
    {
        T ith = data[i];
        T jth = data[j];
        
        data[i] = jth;
        data[j] = ith;
        
        map[ith] = j;
        map[jth] = i;
    }

    bool isLarger(const T& t, const T& o) const
    {
        return t > o;
    }
};
