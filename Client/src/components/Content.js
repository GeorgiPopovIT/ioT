import CardChart from "./CardChart";
import CardGraphic from "./CardGraphic";
import CardItemList from "./CardItemList";
import Statistics from "./Statistics";

const Content = () => {
    return(
    <div className="content">
        <CardItemList/>
        <CardGraphic/>
        <div className="row">
            <Statistics/>
            <CardChart/>
        </div>
    </div>
      
    )
}

export default Content;