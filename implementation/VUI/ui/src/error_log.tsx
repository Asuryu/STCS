import React, { useEffect } from 'react';

interface ToastProps {
    message: string;
    onClose: () => void;
}

const Toast: React.FC<ToastProps> = ({ message, onClose }) => {
    useEffect(() => {
        const timer = setTimeout(() => {
            onClose();
        }, 3000); // Toast will disappear after 3 seconds

        return () => clearTimeout(timer);
    }, [onClose]);

    return (
        <div className="fixed top-4 right-4 z-50">
            <div className="bg-red-600 text-white p-4 border-2 border-red-800 rounded-lg shadow-lg max-w-xs">
                <span>{message}</span>
            </div>
        </div>
    );
};

export default Toast;
