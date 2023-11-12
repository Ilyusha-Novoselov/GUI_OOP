#include "main_distribution.h"
#include "spec_func.hpp"

namespace statistics {

	MainDistribution::MainDistribution(std::ifstream& inFile) {
		// �����������, ��������� ������ �� ������ ��������� �� �����
		read_from_file(inFile);

	};

	double MainDistribution::get_density_distribution(double inX) const {
		// �������, ������������ �������� ��������� ������������ ����������� (double) � ����� inX (double)
		if (_dist_params.v <= 0.5)
			throw _dist_params;

		auto standart_distribution{ [](double x, double v) {return (1 / betta(v - 0.5, 0.5)) * pow(1 + x * x, -v); } };

		if (_dist_params.mu == 0 && _dist_params.lambda == 1)
			standart_distribution(inX, _dist_params.v);

		return standart_distribution(((inX - _dist_params.mu) / _dist_params.lambda), _dist_params.v) / _dist_params.lambda;

	};

	std::optional<double> MainDistribution::get_expected_value() const {
		// �������, ������������ �������� ��������������� �������� ������������� (double), ���� ��� ����������
		// ��� �������� �� ������������� ������������ std::optional
		if ((2 * _dist_params.v - 1) <= 1)
			return std::nullopt;

		return 0 + _dist_params.mu;

	};

	std::optional<double> MainDistribution::get_dispersion() const {
		// �������, ������������ �������� ��������� ������������� (double), ���� ��� ����������
		// ��� �������� �� ������������� ������������ std::optional
		if ((2 * _dist_params.v - 1) <= 2)
			return std::nullopt;

		if (_dist_params.v == 1.5 || _dist_params.lambda <= 0)
			throw _dist_params;

		return (1 / (2 * _dist_params.v - 3)) * pow(_dist_params.lambda, 2);

	};

	std::optional<double> MainDistribution::get_krutosis() const {
		// �������, ������������ �������� ����������� �������� ������������� (double), ���� �� ����������
		// ��� �������� �� ������������� ������������ std::optional
		if ((2 * _dist_params.v - 1) <= 4)
			return std::nullopt;

		if (_dist_params.v == 2.5)
			throw _dist_params;

		return 6 / (2 * _dist_params.v - 5);

	}

	double MainDistribution::generate_random_variable() const {
		// �������, ������������ �������� ��������� �������� � ������������ � ������� �������������� ��������������
		if (_dist_params.v <= 0.5)
			throw _dist_params;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<double> rand_dist(std::nextafter(0.0, 1.0), std::nextafter(1.0, 0.0));

		double x = sqrt(pow(rand_dist(gen), -1 / (_dist_params.v - 0.5)) - 1) * cos(2 * std::atan(1) * 4 * rand_dist(gen));

		return _dist_params.mu + _dist_params.lambda * x;

	};

	void MainDistribution::write_to_file(std::ofstream& outFile) const {
		// �������, ������������ ������� ��������� ������� � ���� (�� ���� ������� ������ std::ofstream outFile)
		// � �������������� ��������������� �������
		if (outFile.is_open()) {
			outFile << _dist_params.v << std::endl;
			outFile << _dist_params.mu << std::endl;
			outFile << _dist_params.lambda << std::endl;
		}
	}

	void MainDistribution::read_from_file(std::ifstream& inFile) {
		// �������, ����������� ��������� ������� �� ����� (�� ���� ������� ������ std::ifstream imFile)
		// � �������������� ��������������� �������
		std::string v, mu, lambda;

		if (inFile.is_open()) {

			std::getline(inFile, v);
			std::getline(inFile, mu);
			std::getline(inFile, lambda);

			_dist_params.v = std::stod(v);
			_dist_params.mu = std::stod(mu);
			_dist_params.lambda = std::stod(lambda);
			//inFile.close();
		};
	};

}; // namespace statistics
