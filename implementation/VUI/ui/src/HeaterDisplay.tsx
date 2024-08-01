import React from "react";
import { FaThermometerHalf } from "react-icons/fa";

interface HeaterDisplayProps {
  temperature: number;
  index: number;
}

const HeaterDisplay: React.FC<HeaterDisplayProps> = ({ temperature, index }) => {
  const temperatureColor = 
    temperature < 20 ? "bg-blue-200" : 
    temperature < 35 ? "bg-green-200" : 
    temperature < 50 ? "bg-yellow-200" : 
    "bg-red-200";

  return (
    <div className={`flex items-center p-4 ${temperatureColor} rounded-lg shadow-lg w-full`}>
      <div className="flex items-center justify-center w-16 h-16 rounded-full bg-white mr-4">
        <FaThermometerHalf className="text-gray-700 text-3xl" />
      </div>
      <div>
        <h3 className="text-lg font-bold text-gray-700">Aquecedor {index + 1}</h3>
        <p className="text-3xl font-semibold text-gray-900">{temperature.toFixed(1)}°C</p>
      </div>
    </div>
  );
};

export default HeaterDisplay;
