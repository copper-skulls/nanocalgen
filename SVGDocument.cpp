#include "SVGDocument.hpp"
#include <fstream>

SVGRoot::~SVGRoot() {};
SVGText::~SVGText() {};
SVGGroup::~SVGGroup() {};
SVGRectangle::~SVGRectangle() {};

void SVGRoot::print(std::ostream &os) const {
  // Print contents of SVGRoot in proper xml format to `os`.
  os << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>" << std::endl;
  os << "<svg xmlns=\"" + SVG_XMLNS + "\" ";
  os << "width=\"" + std::to_string(width) + "\" ";
  os << "height=\"" + std::to_string(height) + "\">" << std::endl;

  for (int i=0; i < children.size(); ++i) {
    children[i]->print(os);
  }

  os << "</svg>" << std::endl;
}

void SVGText::print(std::ostream &os) const {
  // Print contents of SVGText in proper xml format to `os`.
  os << "<text ";
  os << "x=\"" + std::to_string(x) + "\" ";
  os << "y=\"" + std::to_string(y) + "\">";
  os << text;
  os << "</text>" << std::endl;
}

void SVGGroup::print(std::ostream &os) const {
  // Print contents of SVGGroup in proper xml format to `os`.
  os << "<g";
  for (auto keyvalue : properties) {
    os << " " << keyvalue.first;
    os << "=\"" << keyvalue.second << "\"";
  }
  os << ">" << std::endl;

  for (int i=0; i < children.size(); ++i) {
    children[i]->print(os);
  }

  os << "</g>" << std::endl;
}

void SVGRectangle::print(std::ostream &os) const {
  // Print contents of SVGRectangle in proper xml format to `os`.
  os << "<rect ";
  os << "x=\"" + std::to_string(x) + "\" ";
  os << "y=\"" + std::to_string(y) + "\" ";
  os << "width=\"" + std::to_string(width) + "\" ";
  os << "height=\"" + std::to_string(height) + "\" ";

  if (roundness_x.has_value()) {
    os << "rx=\"" + std::to_string(roundness_x.value()) + "\" ";
  }
  if (roundness_y.has_value()) {
    os << "ry=\"" + std::to_string(roundness_y.value()) + "\"";
  }

  os << "/>" << std::endl;
}
