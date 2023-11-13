#include "emperical_distribution.h"

namespace statistics {

EmpericalDistribution::EmpericalDistribution(int N, const MainDistribution& dist)
{
    _N = N;
    _X = new std::vector<double>;
    _function = new std::map<std::pair<double, double>, double>;
    for (int i = 0; i < N; i++)
        _X->push_back(dist.generate_random_variable());
    *_function = generate_emperical_density_function();
}

EmpericalDistribution::EmpericalDistribution(int N, const MixtureDistribution& dist)
{
    _N = N;
    _X = new std::vector<double>;
    _function = new std::map<std::pair<double, double>, double>;
    for (int i = 0; i < N; i++)
        _X->push_back(dist.generate_random_variable());
    *_function = generate_emperical_density_function();
}

EmpericalDistribution::EmpericalDistribution(int N, const EmpericalDistribution& dist)
{
    _N = N;
    _X = new std::vector<double>;
    _function = new std::map<std::pair<double, double>, double>;
    for (int i = 0; i < N; i++)
        _X->push_back(dist.generate_random_variable());
    *_function = generate_emperical_density_function();
}

EmpericalDistribution::EmpericalDistribution(std::ifstream& inFile)
{
    _X = new std::vector<double>;
    _function = new std::map<std::pair<double, double>, double>;
    read_from_file(inFile);
}

double EmpericalDistribution::get_density_distribution(double inX) const
{
    if (_function->empty())
        return 0.0;
    for (auto emperical : *_function)
        if (inX >= emperical.first.first && inX < emperical.first.second)
            return emperical.second;
    if (inX == _function->rbegin()->first.second)
        return _function->rbegin()->second;
    return 0.0;
}

double EmpericalDistribution::get_expected_value() const
{
    double sum = 0;
    for (auto x : *_X)
        sum += x;
    return sum / _X->size();
}

double EmpericalDistribution::get_dispersion() const
{
    double sum = 0, M = this->get_expected_value();
    for (auto x : *_X)
        sum += pow(x - M, 2);
    return sum / _X->size();
}

double EmpericalDistribution::get_asymmetry() const
{
    double sum = 0, M = this->get_expected_value();
    for (auto x : *_X)
        sum += pow(x - M, 3);
    return sum / (_X->size() * pow(this->get_dispersion(), 1.5));
}

double EmpericalDistribution::get_krutosis() const
{
    double sum = 0, M = this->get_expected_value();
    for (auto x : *_X)
        sum += pow(x - M, 4);
    return (sum / (_X->size() * pow(this->get_dispersion(), 2))) - 3;
}

std::map<std::pair<double, double>, double> EmpericalDistribution::generate_emperical_density_function()
{
    std::map<std::pair<double, double>, double> result; // Сама эмпирическая функция в виде словаря
    const int n = _X->size();
    if (n < 2)
        return result;
    std::sort(_X->begin(), _X->end());
    double x_min = *std::min_element(_X->begin(), _X->end());
    double x_max = *std::max_element(_X->begin(), _X->end());
    int k = ceil(std::log2(n)) + 1; // Кол-во интервалов
    double h = (x_max - x_min) / k; // Шаг
    for (int i = 0; i < k; i++)
    {
        bool flag = false;
        double x_start = x_min + i * h, x_end = x_min + (i + 1) * h;

        if (i == (k - 1) && !(x_max >= x_start && x_max < x_end))
            k++;

        std::pair<double, double> interval = std::make_pair(x_start, x_end);
        for (auto x : *_X)
            if ((x >= x_start && x < x_end) || (i == (k - 1) && x == x_end))
            {
                result[interval] += 1.0 / (n * h);
                flag = true;
            }
        if (!flag)
            result[interval] = 0;
    }
    for(auto& interval: result)
    {
        if(!interval.second)
            interval.second = 1e-10;
    }
    return result;
}

double EmpericalDistribution::generate_random_variable() const
{
    double r, x, sum_p = 0;
    double h = _function->begin()->first.second - _function->begin()->first.first;
    do r = (double)rand() / RAND_MAX; while (r == 0. || r == 1.);
    for (auto& emperical : *_function)
    {
        sum_p += (emperical.second * h);
        if (r <= sum_p)
        {
            std::random_device rd; // Инициализация генератора случайных чисел
            std::mt19937 gen(rd()); // Генератор псевдослучайных чисел
            std::uniform_real_distribution<double> dis(emperical.first.first, emperical.first.second);
            x = dis(gen);
            return x;
        }
    }
    return 0;
}

void EmpericalDistribution::write_to_file(std::ofstream& outFile) const
{
    if (outFile.is_open()) {
        outFile << _N << std::endl;
        for (auto x : *_X)
            outFile << x << " ";
        outFile << std::endl;
        for (auto f : *_function)
            outFile << f.first.first << " " << f.first.second << " " << f.second << std::endl;
    }
}

void EmpericalDistribution::read_from_file(std::ifstream& inFile)
{
    if (!_X->empty())
        _X->clear();
    if (!_function->empty())
        _function->clear();

    std::string N, X, interval;

    if (inFile.is_open()) {
        std::getline(inFile, N);
        _N = std::stoi(N);

        std::getline(inFile, X);
        std::istringstream input(X);
        double x;
        while (input >> x)
            _X->push_back(x);

        while (!inFile.eof())
        {
            std::getline(inFile, interval);
            input = std::istringstream(interval);
            double x_1, x_2, f;
            while (input >> x_1 >> x_2 >> f)
                _function->insert(std::make_pair(std::make_pair(x_1, x_2), f));
        }
        inFile.close();
    }
}
}
