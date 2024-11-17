import {useState} from 'react'
import './App.css'
import SliderComponent from "./SliderComponent.jsx";

function App() {
    return (
        <>
            <div>
                <p className="label">delay</p>
                <SliderComponent pid={window.params.kTime.id}
                                 min={window.params.kTime.min}
                                 max={window.params.kTime.max}/>
                <SliderComponent pid={window.params.kFeedback.id}
                                 min={window.params.kFeedback.min}
                                 max={window.params.kFeedback.max}
                />
                <SliderComponent pid={window.params.kDryWet.id}
                                 min={window.params.kDryWet.min}
                                 max={window.params.kDryWet.max}
                />
            </div>
        </>
    )
}

export default App
