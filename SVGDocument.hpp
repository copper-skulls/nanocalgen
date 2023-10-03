#ifndef HPP_SVG_ELEMENT__
#define HPP_SVG_ELEMENT__

// Structures making up the SVG document.

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <optional>
#include <memory>

struct SVGElement {
  SVGElement() = default; // to ensure compatibility with std::vector
  SVGElement(SVGElement &&) = default; // to ensure compatibility with std::vector
  virtual void print(std::ostream &os = std::cout) const = 0;
  virtual ~SVGElement() {};
};


struct SVGRoot : public SVGElement {
  // SVGElement representing an `svg` element.
  ~SVGRoot();
  SVGRoot() = default; // to ensure compatibility with std::vector
  SVGRoot(SVGRoot &&) = default; // to ensure compatibility with std::vector
  void print(std::ostream &os = std::cout) const;

  int width, height;
  std::vector<std::unique_ptr<SVGElement>> children;

  private: 
    const std::string SVG_XMLNS = "http://www.w3.org/2000/svg";
};

struct SVGText : public SVGElement {
  // SVGElement representing a `text` element.
  ~SVGText();
  SVGText() = default; // to ensure compatibility with std::vector
  SVGText(SVGText &&) = default; // to ensure compatibility with std::vector
  void print(std::ostream &os = std::cout) const;

  int x, y;
  std::string text;
};

struct SVGGroup : public SVGElement {
  // SVGElement representing a `g` element.
  // Since the element is very general and can just
  // passes along its properties to its children,
  // uses a general property dictionary instead
  // of anything with checks.
  ~SVGGroup();
  SVGGroup() = default; // to ensure compatibility with std::vector
  SVGGroup(SVGGroup &&) = default; // to ensure compatibility with std::vector
  void print(std::ostream &os = std::cout) const;

  std::map<std::string, std::string> properties;
  std::vector<std::unique_ptr<SVGElement>> children;
};

struct SVGRectangle : public SVGElement {
  // SVGElement representing a `rect` xml tag.
  ~SVGRectangle();
  SVGRectangle() = default; // to ensure compatibility with std::vector
  SVGRectangle(SVGRectangle &&) = default; // to ensure compatibility with std::vector
  void print(std::ostream &os = std::cout) const;

  int x, y, width, height;
  std::optional<int> roundness_x, roundness_y;
};
#endif
