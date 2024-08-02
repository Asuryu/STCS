import React from "react";
import { FaThermometerHalf } from "react-icons/fa";

interface HeaterDisplayProps {
  temperature: number;
  index: number;
}

const HeaterDisplay: React.FC<HeaterDisplayProps> = ({ temperature, index }) => {
  // Determine background color based on temperature
  const temperatureColor = 
    temperature < 20 ? "bg-blue-100" : 
    temperature < 35 ? "bg-green-100" : 
    temperature < 50 ? "bg-yellow-100" : 
    "bg-red-100";

  return (
    <div className={`flex items-center p-2 ${temperatureColor} rounded-lg shadow-sm max-w-xs`}>
      <div className="flex items-center justify-center w-12 h-12 rounded-full bg-white mr-3">
        <FaThermometerHalf className="text-gray-700 text-xl" />
      </div>
      <div>
        <h3 className="text-sm font-semibold text-gray-700">Heater {index + 1}</h3>
        <p className="text-l font-bold text-gray-900">{temperature.toFixed(1)}°C</p>
      </div>
    </div>
  );
};

export default HeaterDisplay;

