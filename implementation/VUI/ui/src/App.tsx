import { useEffect } from 'react'
import Dashboard from './Dashboard'

function App() {
  const handleSocket = () => {
    const ws = new WebSocket('ws://server:8084')
    ws.onopen = () => {
      console.log('connected')
    }
    ws.onmessage = (msg) => {
      console.log(msg.data)
    }
    ws.onclose = () => {
      console.log('disconnected')
    }
  }

  useEffect(() => {
    handleSocket();
  }, [])

  return (
    <>
      <div className="bg-gray-50 min-h-screen flex items-center justify-center">
        <Dashboard />
      </div>
    </>
  )
}

export default App;
